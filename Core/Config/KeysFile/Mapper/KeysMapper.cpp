// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "KeysMapper.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Resolve/Types/ResolverCandidate.h"

#include <cstddef>
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

int keyFromText(const std::string& text) {
        return QKeySequence(QString::fromStdString(text))[0].key();
}

keybindings keysFromText(const std::vector<std::string>& texts) {
        keybindings keys = {};
        keys.reserve(texts.size());

        for (const std::string& text : texts) { keys.insert(keyFromText(text)); }

        return keys;
}

std::vector<std::string> textFromTomlArray(const toml::array& arr) {
        std::vector<std::string> texts = {};
        texts.reserve(arr.size());

        for (const auto& element : arr) {
                if (const auto* str_element = element.as_string()) {
                        texts.push_back(str_element->get());
                }
        }

        return texts;
}

keybindings config::KeysMapper::quit(const Candidates& candidates, const keybindings& defaults,
                                     const PathContext& path_context) {
        const auto* keys = resolve::fromAs<toml::array>(candidates, path_context, {.min_size = 1},
                                                        u"[keybindings...]");

        if (!keys || keys->empty()) { return defaults; }

        return keysFromText(textFromTomlArray(*keys));
}

std::vector<keybindings> config::KeysMapper::primaryButtons(const Candidates& candidates,
                                                            const std::vector<keybindings>& defaults,
                                                            const PathContext& path_context) {
        const auto* keys = resolve::fromAs<toml::array>(candidates, path_context, {.min_size = 1},
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

        return std::move(buttons);
}

keybindings config::KeysMapper::primaryButton(const Candidates&  candidates,
                                              const keybindings& defaults,
                                              const PathContext& path_context) {
        const auto* keys = resolve::fromAs<toml::array>(candidates, path_context, {.min_size = 1},
                                                        u"[keybindings...]");

        if (keys->empty()) { return defaults; }

        return keysFromText(textFromTomlArray(*keys));
}
