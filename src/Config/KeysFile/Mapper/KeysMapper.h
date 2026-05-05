// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Types/NodeView.h"

#include <string>
#include <toml++/toml.hpp>
#include <vector>

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
                                const PathContext& path_context);

        /* Power Applet Keys*/
        static void mapPrimaryButtonKey(node_view primary_button_node, keybindings& primary_button,
                                        const keybindings& defaults,
                                        const PathContext& path_context);
        static void mapPrimaryButtonKeys(node_view                       primary_buttons_node,
                                         std::vector<keybindings>&       primary_buttons,
                                         const std::vector<keybindings>& defaults,
                                         const PathContext&              path_context);

public:
        KeysMapper() = delete;
        template<typename TKeys>
        static void mapToPowerAppletKeys(const toml::table& power_applet_table,
                                         const toml::table& global_table, TKeys& keys);
};

#include "KeysMapper.tpp"
