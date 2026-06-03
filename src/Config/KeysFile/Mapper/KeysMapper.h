// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/Types/NodeView.h"

#include <string>
#include <toml++/toml.hpp>
#include <vector>

class GlobalKeys;
class PathContext;
class ResolverCandidates;
class PowerAppletKeys;
class QString;

// Parses key name strings (e.g. "Ctrl+A") into a keybindings set, stripped of
// modifiers.
keybindings keysFromText(const std::vector<std::string>& texts);

// Extracts string elements from a toml::array, silently skipping non-string values.
std::vector<std::string> textFromTomlArray(const toml::array& arr);

// Maps TOML configuration to PowerAppletKeys structure.
//
// Note: This file only handles PowerAppletKeys mapping. GlobalKeys is only used
//       as a fallback candidate but not mapped to any struct.
//
// All mapping failures will fall back to defaults and log warnings.
class KeysMapper final {
private:
        /* Global Keys */

        // Maps quit keybindings from config nodes.
        //
        // Fallback priority: power_applet.quit -> global.quit -> hardcoded defaults
        //
        // Expected format: array of keybinding strings, minimum 1 element.
        //
        // Assigned value: keybindings (std::unordered_set<int>)
        static keybindings quit(const ResolverCandidates& candidates, const keybindings& defaults,
                                const PathContext& path_context);

        /* Power Applet Keys*/

        // Maps the entire primary_buttons array from config nodes.
        //
        // Length of the vector may differ from defaults if some buttons are omitted
        // from config. Omitted buttons are ignored silently.
        //
        // Fallback priority: power_applet.primary_buttons -> hardcoded defaults
        //
        // Expected format: array of primary_buttons tables, each containing
        //                  keybindings
        //
        // Assigned value: vector of keybindings (std::unordered_set<int>)
        static std::vector<keybindings> primaryButtons(const ResolverCandidates&       candidates,
                                                       const std::vector<keybindings>& defaults,
                                                       const PathContext& path_context);

        // Maps a single button's keybindings from a config candidate.
        //
        // Fallback priority: power_applet.primary_buttons[index] -> hardcoded defaults
        //
        // Expected format: string representation of keybinding
        //
        // Assigned value: keybindings (std::unordered_set<int>)
        static keybindings primaryButton(const ResolverCandidates& candidates,
                                         const keybindings&        defaults,
                                         const PathContext&        path_context);

public:
        KeysMapper() = delete;

        // Parses power_applet and global tables into PowerAppletKeys.
        //
        // Usage:
        //   PowerAppletKeys keys;
        //   KeysMapper::keys(power_applet_table, global_table, keys);
        //
        // The power_applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling (initialized in main()).
        //
        // Only PowerAppletKeys is supported. Template arg TKeys is used to break a
        // circular dependency between this header and PowerAppletKeys.h by deferring
        // the instantiation of PowerAppletKeys.
        template<typename TKeys>
        static TKeys keys(const toml::table& applet, const toml::table& global,
                          const TKeys& defaults);
};

#include "KeysMapper.tpp"
