// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Map.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Resolve/Types/Candidate.h"
#include "Core/Config/Types/NodeView.h"

#include <cstddef>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QKeySequence>
#include <QString>

using namespace config;
using config::resolve::Candidates;
using config::resolve::PathContext;

/* Helpers */

std::optional<int> keyFromText(const std::string& text) {
        return QKeySequence(QString::fromStdString(text))[0].key();
}

std::optional<int> keyFromTomlElement(node_view element) {
        const auto* str = element.as_string();

        if (!str) { return std::nullopt; }

        return keyFromText(str->get());
}

keybindings keysFromTomlArray(const toml::array& arr) {
        keybindings keys = {};
        keys.reserve(arr.size());

        for (const auto& element : arr) {
                if (auto key = keyFromTomlElement(node_view(element))) { keys.insert(key.value()); }
        }

        return keys;
}

/* KeysMapper */

keybindings config::map::quit(const Candidates& candidates, const keybindings& defaults,
                              const PathContext& path_context) {
        const auto* keys = resolve::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                        u"[keybindings...]");

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys);
}

std::vector<keybindings> config::map::primaryButtons(const Candidates&               candidates,
                                                     const std::vector<keybindings>& defaults,
                                                     const PathContext&              path_context) {
        const auto* keys = resolve::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                        u"[keybindings...]");

        if (!keys || keys->empty()) { return defaults; }

        std::vector<keybindings> buttons = {};
        buttons.reserve(keys->size());

        for (size_t i = 0; i != keys->size(); ++i) {
                keybindings found_for_button = primaryButton(candidates.makeCopy().withExtension(i),
                                                             defaults[i],
                                                             path_context.makeExtended(i));
                if (!found_for_button.empty()) { buttons.push_back(std::move(found_for_button)); }
        }

        return buttons;
}

keybindings config::map::primaryButton(const Candidates& candidates, const keybindings& defaults,
                                       const PathContext& path_context) {
        const auto* keys = resolve::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                        u"[keybindings...]");

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys);
}
