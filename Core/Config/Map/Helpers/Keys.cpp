// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keys.h"

#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/Config/Types/NodeView.h"

#include <QKeySequence>
#include <QString>
#include <TomlQt/ArrayBounds.h>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>

using namespace config;

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

/* Keys Schema */

keybindings config::map::helpers::quit(const Candidates& candidates, const keybindings& defaults) {
        const auto* keys = resolve::from<toml::array>(
                candidates, {.bounds = ArrayBounds{.min_size = 1}, .format = u"[keybindings...]"});

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys);
}

std::vector<keybindings> config::map::helpers::primaryButtons(
        const Candidates& candidates, const std::vector<keybindings>& defaults) {
        const auto* keys = resolve::from<toml::array>(
                candidates, {.bounds = ArrayBounds{.min_size = 1}, .format = u"[keybindings...]"});

        if (!keys || keys->empty()) { return defaults; }

        std::vector<keybindings> buttons = {};
        buttons.reserve(keys->size());

        for (size_t i = 0; i != keys->size(); ++i) {
                keybindings found_for_button = primaryButton(candidates[i], defaults[i]);
                if (!found_for_button.empty()) { buttons.push_back(std::move(found_for_button)); }
        }

        return buttons;
}

keybindings config::map::helpers::primaryButton(
        const Candidates& candidates, const keybindings& defaults) {
        const auto* keys = resolve::from<toml::array>(
                candidates, {.bounds = ArrayBounds{.min_size = 1}, .format = u"[keybindings...]"});

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys);
}
