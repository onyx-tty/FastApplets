// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <string>
#include <toml++/toml.hpp>
#include <vector>

namespace config::resolve {
class Candidates;
class PathContext;
} // namespace config::resolve

class QString;

// Parses key name strings (e.g. "Ctrl+A") into a keybindings set, stripped of
// modifiers.
keybindings keysFromText(const std::vector<std::string>& texts);

// Extracts string elements from a toml::array, silently skipping non-string values.
std::vector<std::string> textFromTomlArray(const toml::array& arr);

// Maps TOML configuration to config::schema::Keys structure.
//
// All mapping failures will fall back to defaults and log warnings.
class KeysMapper final {
private:
        using Candidates  = config::resolve::Candidates;
        using PathContext = config::resolve::PathContext;

        // Maps quit keybindings from config nodes.
        //
        // Fallback priority: applet.quit -> global.quit -> hardcoded defaults
        //
        // Return value: keybindings (std::unordered_set<int>)
        static keybindings quit(const Candidates& candidates, const keybindings& defaults,
                                const PathContext& path_context);

        // Maps the entire primary_buttons array from config nodes.
        //
        // Length of the vector may differ from defaults if some buttons are omitted
        // from config. Omitted buttons are ignored silently.
        //
        // Fallback priority: applet.primary_buttons -> hardcoded defaults
        //
        // Return value: std::vector<keybindings> (std::vector<std::unordered_set<int>>)
        static std::vector<keybindings> primaryButtons(const Candidates&               candidates,
                                                       const std::vector<keybindings>& defaults,
                                                       const PathContext& path_context);

        // Maps a single button's keybindings from a config candidate.
        //
        // Fallback priority: applet.primary_buttons[index] -> hardcoded defaults
        //
        // Return value: keybindings (std::unordered_set<int>)
        static keybindings primaryButton(const Candidates& candidates, const keybindings& defaults,
                                         const PathContext& path_context);

public:
        KeysMapper() = delete;

        // Parses applet and global tables into Keys.
        //
        // Usage:
        //   auto keys = KeysMapper::keys(applet, global, defaults);
        //
        // The applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling (initialized in main()).
        //
        // Return value: config::schema::Keys
        template<applet::type TApplet>
        static config::schema::Keys keys(const toml::table& applet, const toml::table& global,
                                         const config::schema::Keys& defaults);
};

#include "KeysMapper.tpp"
