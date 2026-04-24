/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "KeysMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/Resolvers/Resolvers.h"
#include "Config/TOML/TomlAccessor.h"
#include "Config/TOML/Types/NodePair.h"
#include "Config/TOML/Types/NodeView.h"
#include "Config/TOML/Types/TomlArrayConditions.h"
#include "Log/Log.h"

#include <cstddef>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QKeyCombination>
#include <QKeySequence>
#include <QString>

int interpretTextAsHex(const std::string& text) {
        QKeySequence    sequence(QString::fromStdString(text));
        QKeyCombination combination(sequence[0]);

        return combination.key();
};

keybindings interpretTextAsKeybindings(const std::vector<std::string>& text_list) {
        keybindings keys{};
        keys.reserve(text_list.size());
        for (const std::string& text : text_list) { keys.insert(interpretTextAsHex(text)); }

        return keys;
}

std::vector<std::string> interpretTomlArrayAsStringVector(const toml::array& toml_array) {
        std::vector<std::string> str_vec{};
        str_vec.reserve(toml_array.size());

        for (const auto& element : toml_array) {
                if (const auto& str_element = element.as_string()) {
                        str_vec.push_back(str_element->get());
                }
        }

        return str_vec;
}

void KeysMapper::mapQuitKeys(NodePair nodes, keybindings& quit, const keybindings& defaults,
                             const QString& path_context) {
        constexpr size_t min_size = 1;

        toml::array array{};
        resolveOrDefault<toml::array>({Source{nodes.primary, applet::power_applet.scope},
                                       Source{nodes.fallback, applet::global.scope}},
                                      array, quit, defaults, path_context,
                                      {"Format: [keybindings...]", min_size, std::nullopt});

        quit = interpretTextAsKeybindings(interpretTomlArrayAsStringVector(array));
}

void KeysMapper::mapPrimaryButtonKey(node_view primary_button_node, keybindings& primary_button,
                                     const keybindings& defaults, const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size    = 1;

        toml::array button{};
        resolveOrDefault<toml::array>({Source{primary_button_node, applet::power_applet.scope}},
                                      button, primary_button, defaults, path_context,
                                      {"Format: [keybindings...]", min_size, std::nullopt});

        primary_button = interpretTextAsKeybindings(interpretTomlArrayAsStringVector(button));
}

void KeysMapper::mapPrimaryButtonKeys(node_view                       primary_buttons_node,
                                      std::vector<keybindings>&       primary_buttons,
                                      const std::vector<keybindings>& defaults,
                                      const QString&                  path_context) {
        constexpr size_t min_size = 1;
        const size_t     max_size = primary_buttons.size();

        toml::array primary_button_arr{};
        resolveOrDefault({Source{primary_buttons_node, applet::power_applet.scope}},
                         primary_button_arr, primary_buttons, defaults, path_context,
                         {"Format: [keybindings...]", min_size, std::nullopt});

        std::vector<keybindings> primary_buttons_new{};
        for (size_t i = 0; i != primary_button_arr.size(); ++i) {
                keybindings keys{};
                mapPrimaryButtonKey(primary_buttons_node[i], keys, defaults[i],
                                    path_context + QString("[%1]").arg(i).toStdString().c_str());
                if (!keys.empty()) { primary_buttons_new.push_back(keys); }
        };

        primary_buttons = std::move(primary_buttons_new);
}
