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

#pragma once

#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/TOML/Types/NodeView.h"
#include "Global/GlobalKeys.h"
#include "PowerApplet/PowerAppletKeys.h"

#include <array>
#include <toml++/toml.hpp>

class GlobalKeys;
class NodePair;
class PowerAppletKeys;
class QString;

/* Interpret a string representation of a keyboard shortcut as a Qt key code */
// Given a vector of text:
// 1. Parse each element into a QKeySequence
// 2. Extract just the key (without modifiers) from the key combination,
//    as a hexadecimal value
int interpretTextAsHex(const std::string& text);

/* Apply interpretTextAsHex to each element of a string vector */
keybindings interpretTextAsKeybindings(node_view source, keybindings& target);

/* Validate a toml::array, interpret as a string vector, discard non-string elements */
std::vector<std::string> interpretTomlArrayAsStringVector(const toml::array& toml_array);

class KeysMapper final {
private:
        /* Global Keys */
        static void mapQuitKeys(NodePair nodes, keybindings& quit, const keybindings& defaults,
                                const QString& path_context);

        /* Power Applet Keys*/
        static void mapPrimaryButtonKey(node_view primary_button_node, keybindings& primary_button,
                                        const keybindings& defaults, size_t primary_buttons_size,
                                        const QString& path_context);
        static void mapPrimaryButtonKeys(node_view                         primary_buttons_node,
                                         std::array<keybindings, 4>&       primary_buttons,
                                         const std::array<keybindings, 4>& defaults,
                                         const QString&                    path_context);

public:
        KeysMapper() = delete;
        static void mapToPowerAppletKeys(const toml::table& power_applet_table,
                                         const toml::table& global_table, PowerAppletKeys& keys);
};
