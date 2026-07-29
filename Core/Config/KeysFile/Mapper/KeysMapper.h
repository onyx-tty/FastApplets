// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <string>
#include <toml++/toml.hpp>
#include <vector>

namespace config::resolve {
class Candidates;
class PathContext;
} // namespace config::resolve

// Parses key name strings (e.g. "Ctrl+A") into a keybindings set, stripped of
// modifiers.
[[nodiscard]] keybindings keysFromText(const std::vector<std::string>& texts);

// Extracts string elements from a toml::array, silently skipping non-string values.
[[nodiscard]] std::vector<std::string> textFromTomlArray(const toml::array& arr);

namespace config {

// Maps TOML configuration to config::schema::Keys structure.
//
// All mapping failures will fall back to defaults and log warnings.
class KeysMapper final {
private:
        using Keys        = config::schema::Keys;
        using Candidates  = config::resolve::Candidates;
        using PathContext = config::resolve::PathContext;

        // Maps quit keybindings from a list of candidates.
        //
        // Return value: keybindings (std::unordered_set<int>)
        [[nodiscard]] static keybindings quit(const Candidates&  candidates,
                                              const keybindings& defaults,
                                              const PathContext& path_context);

        // Maps the entire primary_buttons array from a list of candidates.
        //
        // Length of the vector may differ from defaults if some buttons are omitted
        // from config. Omitted buttons are ignored silently.
        //
        // Return value: std::vector<keybindings> (std::vector<std::unordered_set<int>>)
        [[nodiscard]] static std::vector<keybindings> primaryButtons(
                const Candidates& candidates, const std::vector<keybindings>& defaults,
                const PathContext& path_context);

        // Maps a single button's keybindings from a list of candidates.
        //
        // Return value: keybindings (std::unordered_set<int>)
        [[nodiscard]] static keybindings primaryButton(const Candidates&  candidates,
                                                       const keybindings& defaults,
                                                       const PathContext& path_context);

public:
        KeysMapper() = delete;

        // Parses applet and global tables into Keys.
        //
        // Usage:
        //   auto keys = KeysMapper::keys<TApplet>(applet, global, defaults);
        //
        // The applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling.
        //
        // Return value: config::schema::Keys
        template<applet::type TApplet>
        [[nodiscard]] static Keys keys(const toml::table& applet, const toml::table& global,
                                       const Keys& defaults);
};

} // namespace config

#include "KeysMapper.tpp"
