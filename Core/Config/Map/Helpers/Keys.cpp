// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keys.h"

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

keybindings keysFromTomlArray(const toml::array& arr, keybindings& claimed_keys) {
        keybindings keys = {};
        keys.reserve(arr.size());

        for (const auto& element : arr) {
                if (auto key = keyFromTomlElement(node_view(element))) {
                        claimKeys(keys, claimed_keys, {key.value()});
                }
        }

        return keys;
}

/* Keys Schema */

keybindings config::map::helpers::quit(
        const ConfigView& node, const keybindings& defaults, keybindings& claimed_keys) {
        ArrayBounds bounds = {.min_size = 1};

        const auto* keys = node.resolve<toml::array>(bounds);

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys, claimed_keys);
}

std::vector<keybindings> config::map::helpers::primaryButtons(const ConfigView& node,
        const std::vector<keybindings>& defaults, keybindings& claimed_keys) {
        ArrayBounds bounds = {.min_size = 1};

        const auto* keys = node.resolve<toml::array>(bounds);

        if (!keys || keys->empty()) { return defaults; }

        std::vector<keybindings> buttons = {};
        buttons.reserve(keys->size());

        for (int i = 0; i != keys->size(); ++i) {
                keybindings found_for_button = primaryButton(node[i], defaults[i], claimed_keys);
                if (!found_for_button.empty()) { buttons.push_back(std::move(found_for_button)); }
        }

        return buttons;
}

keybindings config::map::helpers::primaryButton(
        const ConfigView& node, const keybindings& defaults, keybindings& claimed_keys) {
        ArrayBounds bounds = {.min_size = 1};

        const auto* keys = node.resolve<toml::array>(bounds);

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys, claimed_keys);
}
