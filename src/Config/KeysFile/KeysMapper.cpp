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
// TODO Extract into TOML utilities
// TODO Split into separate functions:
// TODO 1. Error-handling
// TODO 2. Returning an array of 'keybindings'
// TODO 3. Assigning the array of 'keybindings' to the target
void interpretTextAsKeybindings(node_view source, keybindings& target) {
        /* Interpret a string representation of a keyboard shortcut to a Qt key code */
        // Given a node containing a string like "Escape", this lambda:
        // 1. Extracts the string from the node
        // 2. Parses it as a QKeySequence, which in this case is recognized as Qt::Key_Escape
        // 3. Extracts just the key (without modifiers) from the key combination,
        //    as a hexadecimal value
        // TODO Error handling, ensure the node is valid, handle possible failed conversions
        // TODO Extract into TOML utilities
        const auto textToHexInterpreter = [](const auto& node) {
                QKeySequence    sequence(QString::fromStdString(node.as_string()->get()));
                QKeyCombination combination(sequence[0]);

                return combination.key();
        };

        // Validate source
        if (!source) {
                QCRITICAL() << "Source doesn't exist!";
                return;
        }

        if (const auto& keys_raw = source.as_array()) {
                // Parse each key shortcut string representation into a corresponding keybinding
                // and insert it at the target
                target.reserve(keys_raw->size());
                for (const auto& key : *keys_raw) { target.insert(textToHexInterpreter(key)); }
        } else {
                QCRITICAL() << "Source must be an array!";
                return;
        }
}

void KeysMapper::mapQuitKeys(NodePair nodes, keybindings& quit, const keybindings& defaults,
                             const QString& path_context) {
        constexpr bool is_override = false;
        const auto     array       = resolve(path_context, is_override, "Format: [keybindings...]",
                                             std::nullopt, std::nullopt,
                                             Source{nodes.primary, applet::power_applet.scope},
                                             Source{nodes.fallback, applet::global.scope});

        if (!array || array.value().empty()) {
                quit = defaults;
                return;
        }

        interpretTextAsKeybindings(node_view(array.value()), quit);
}

void KeysMapper::mapPrimaryButtonKey(node_view primary_button_node, keybindings& primary_button,
                                     const keybindings& defaults, size_t primary_buttons_size,
                                     const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size    = 1;

        const auto button = resolve(path_context, is_override, "Format: [keybindings...]", min_size,
                                    primary_buttons_size,
                                    Source{primary_button_node, applet::power_applet.scope});
        if (!button) {
                primary_button = defaults;
                return;
        }

        interpretTextAsKeybindings(primary_button_node, primary_button);
}

void KeysMapper::mapPrimaryButtonKeys(node_view                         primary_buttons_node,
                                      std::array<keybindings, 4>&       primary_buttons,
                                      const std::array<keybindings, 4>& defaults,
                                      const QString&                    path_context) {
        constexpr bool   is_override          = false;
        constexpr size_t max_size             = primary_buttons.size();
        const auto       primary_button_nodes = resolve(path_context, is_override,
                                                        "Format: [keybindings...]", std::nullopt,
                                                        max_size,
                                                        Source{primary_buttons_node,
                                                               applet::power_applet.scope});

        if (!primary_button_nodes || primary_button_nodes.value().size() < max_size) {
                primary_buttons = defaults;
                return;
        }

        std::array<keybindings, max_size> primary_buttons_new{};
        for (size_t i = 0; i != primary_button_nodes.value().size(); ++i) {
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
