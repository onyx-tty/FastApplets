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
#include "Core/Log.h"
#include "Global/GlobalKeys.h"
#include "PowerApplet/PowerAppletKeys.h"

#include <array>
#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>
#include <utility>
#include <QApplication>
#include <QKeyCombination>
#include <QKeySequence>
#include <QString>

/* Interpret an array of string representations of a keyboard shortcut at a target location */
// Apply the internal lambda textToHexInterpreter to each keyboard shortcut string representation
// Assign the result to the given target
void interpretTextAsKeybindings(const std::vector<std::string>& text_list, keybindings& target) {
        /* Interpret a string representation of a keyboard shortcut as a Qt key code */
        // Given a vector of text, this lambda:
        // 1. Parses each into a QKeySequence
        // 2. Extracts just the key (without modifiers) from the key combination,
        //    as a hexadecimal value
        const auto textToHexInterpreter = [](const std::string& text) {
                QKeySequence    sequence(QString::fromStdString(text));
                QKeyCombination combination(sequence[0]);

                return combination.key();
        };

        // Parse each key shortcut string representation into a corresponding keybinding
        // and insert it at the target
        target.reserve(text_list.size());
        for (const std::string& text : text_list) { target.insert(textToHexInterpreter(text)); }
}

std::vector<std::string> interpretTomlArrayAsStringVector(const toml::array& toml_array) {
        std::vector<std::string> str_vec{};
        str_vec.reserve(toml_array.size());

        for (const auto& element : toml_array) {
                if (const auto& str_element = element.as_string()) {
                        str_vec.push_back(str_element->get());
                }
        }

        return std::move(str_vec);
}

void KeysMapper::mapQuitKeys(NodePair nodes, keybindings& quit, const keybindings& defaults,
                             const QString& path_context) {
        constexpr size_t min_size = 1;

        toml::array array{};
        resolveOrDefault<keybindings>(path_context, array, quit, defaults,
                                      "Format: [keybindings...]", min_size, std::nullopt,
                                      Source{nodes.primary, applet::power_applet.scope},
                                      Source{nodes.fallback, applet::global.scope});

        interpretTextAsKeybindings(interpretTomlArrayAsStringVector(array), quit);
}

void KeysMapper::mapPrimaryButtonKey(node_view primary_button_node, keybindings& primary_button,
                                     const keybindings& defaults, size_t primary_buttons_size,
                                     const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size    = 1;

        toml::array button{};
        resolveOrDefault(path_context, button, primary_button, defaults, "Format: [keybindings...]",
                         min_size, primary_buttons_size,
                         Source{primary_button_node, applet::power_applet.scope});

        interpretTextAsKeybindings(interpretTomlArrayAsStringVector(button), primary_button);
}

void KeysMapper::mapPrimaryButtonKeys(node_view                         primary_buttons_node,
                                      std::array<keybindings, 4>&       primary_buttons,
                                      const std::array<keybindings, 4>& defaults,
                                      const QString&                    path_context) {
        constexpr size_t min_size = 1, max_size = primary_buttons.size();

        toml::array primary_button_arr{};
        resolveOrDefault(path_context, primary_button_arr, primary_buttons, defaults,
                         "Format: [keybindings...]", min_size, max_size,
                         Source{primary_buttons_node, applet::power_applet.scope});

        std::array<keybindings, max_size> primary_buttons_new{};
        for (size_t i = 0; i != primary_button_arr.size(); ++i) {
                mapPrimaryButtonKey(primary_buttons_node[i], primary_buttons_new[i], defaults[i],
                                    max_size,
                                    extendCfgPath(path_context,
                                                  QString("[%1]").arg(i).toStdString().c_str()));
        };

        primary_buttons = std::move(primary_buttons_new);
}

void KeysMapper::mapToPowerAppletKeys(const toml::table& power_applet_table,
                                      const toml::table& global_table, PowerAppletKeys& keys) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletKeys::getDefault();

        /* Quit Keys */
        mapQuitKeys(NodePair{power_applet_table["quit"], global_table["quit"]}, keys.quit_keys,
                    defaults.getQuitKeys(), "quit");

        /* Primary Button Keys */
        mapPrimaryButtonKeys(power_applet_table["primary_buttons"], keys.primary_button_keys,
                             defaults.getPrimaryButtonKeys(), "primary_buttons");
}
