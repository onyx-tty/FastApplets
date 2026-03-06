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
#include "Core/Log.h"
#include "Keys.h"

#include <array>
#include <string>
#include <toml++/toml.hpp>
#include <QKeySequence>
#include <QKeyCombination>
#include <QString>
#include <QApplication>

/* Interpret an array of string representations of a keyboard shortcut at a target location */
// Apply the internal lambda textToHexInterpreter to each keyboard shortcut string representation
// Assign the result to the given target
// TODO Extract into TOML utilities
// TODO Split into separate functions:
// TODO 1. Error-handling
// TODO 2. Returning an array of 'keybindings'
// TODO 3. Assigning the array of 'keybindings' to the target
void interpretTextAsKeybindings(const toml::node_view<const toml::node>& source,
                                keybindings&                             target) {
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

void KeysMapper::mapGlobalQuitKeys(const toml::table& global, Keys& keys) {
        const auto& data     = (global)["quit"].as_array();
        const auto& defaults = Keys::getDefaultKeys().getGlobalKeys().getQuitKeys();
        auto&       quit     = keys.global_keys.quit_keys;

        if (!data) {
                QWARNING() << "in keys.toml, global.quit must be an array! Using defaults...";
                quit = defaults;
                return;
        }

        interpretTextAsKeybindings(global["quit"], quit);
}

void KeysMapper::mapGlobalKeys(const toml::table& keys_table, Keys& keys) {
        const auto& data     = keys_table["global"].as_table();
        const auto& defaults = Keys::getDefaultKeys().getGlobalKeys();
        auto&       global   = keys.global_keys;

        if (!data) {
                QWARNING() << "in keys.toml, global must be a table! Using defaults...";
                global = defaults;
                return;
        }

        /* Quit Keys */
        mapGlobalQuitKeys(*data, keys);
}

void KeysMapper::mapPowerAppletQuitKeys(const toml::table& power_applet, Keys& keys) {
        const auto& data     = power_applet["quit"].as_array();
        const auto& defaults = Keys::getDefaultKeys().getPowerAppletKeys().getQuitKeys();
        auto&       quit     = keys.power_applet_keys.quit_keys;

        const auto& power_applet_quit = (power_applet)["quit"].as_array();
        if (!power_applet_quit) {
                QWARNING() << "in keys.toml, power_applet.quit must be an array! Using defaults..";
                quit = defaults;
                return;
        }

        // If power_applet.quit not empty, interpret, otherwise copy global_keys.quit
        if (!(*power_applet_quit).empty()) {
                interpretTextAsKeybindings(power_applet["quit"], quit);
        } else {
                keys.power_applet_keys.quit_keys = keys.global_keys.quit_keys;
        }
}

void KeysMapper::mapPowerAppletPrimaryButtonKeys(const toml::table& power_applet, Keys& keys) {
        const std::array<const toml::array*, 4> data = {power_applet["primary_button1"].as_array(),
                                                        power_applet["primary_button2"].as_array(),
                                                        power_applet["primary_button3"].as_array(),
                                                        power_applet["primary_button4"].as_array()};
        const std::array<keybindings, 4>&       defaults =
                Keys::getDefaultKeys().getPowerAppletKeys().getPrimaryButtonKeys();
        auto& buttons = keys.power_applet_keys.primary_button_keys;

        // Parse power_applet.primary_buttonX
        for (size_t i = 0; i != buttons.size(); ++i) {
                std::string button_name = "primary_button" + std::to_string(i + 1);
                const auto& button      = data[i];

                if (!button) {
                        QWARNING_NS() << "in keys.toml, power_applet.primary_button" << i
                                      << " must be an array! Using defaults...";
                        buttons[i] = defaults[i];
                        continue;
                }

                interpretTextAsKeybindings(power_applet[button_name], buttons[i]);
        }
}

void KeysMapper::mapPowerAppletKeys(const toml::table& keys_table, Keys& keys) {
        const auto& data         = keys_table["power_applet"].as_table();
        const auto& defaults     = Keys::getDefaultKeys().getPowerAppletKeys();
        auto&       power_applet = keys.power_applet_keys;

        if (!data) {
                QWARNING() << "in keys.toml, power_applet must be a table! Using defaults...";
                power_applet = defaults;
                return;
        }

        /* Quit Keys */
        mapPowerAppletQuitKeys(*data, keys);

        /* Primary Button Keys*/
        mapPowerAppletPrimaryButtonKeys(*data, keys);
}

void KeysMapper::mapToKeys(const toml::table& keys_table, Keys& keys) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const toml::table& data = keys_table;

        /* Global Keys */
        mapGlobalKeys(data, keys);

        /* PowerApplet Keys*/
        mapPowerAppletKeys(data, keys);
}
