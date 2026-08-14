// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/KeysFile/Schema/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Config/Types/ArraySpec.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

namespace config {
class Candidate;
} // namespace config

namespace config::select {
class PathContext;
} // namespace config::select

// Parses key name strings (e.g. "Ctrl+A") into a keybindings set, stripped of
// modifiers.
// Returns std::nullopt if there is no matching string equivalent.
[[nodiscard]] std::optional<int> keyFromText(const std::string& text);

// Converts nodes into int (Qt::Key).
// Returns std::nullopt on non-string nodes and keys without a matching string equivalent.
[[nodiscard]] std::optional<int> keyFromTomlElement(node_view element);

// Converts toml::array elements into int and returns them as keybindings, silently skipping
// non-string values.
[[nodiscard]] keybindings keysFromTomlArray(const toml::array& arr);

// Maps TOML configuration to config::schema::Keys structure.
//
// All mapping failures will fall back to defaults and log warnings.
namespace config::map {

using config::ArraySpec;
using config::Candidates;
using config::schema::Keys;
using config::select::PathContext;

// Maps quit keybindings from a list of candidates.
//
// Return value: keybindings (std::unordered_set<int>)
[[nodiscard]] keybindings quit(const Candidates& candidates, const keybindings& defaults);

// Maps the entire primary_buttons array from a list of candidates.
//
// Length of the vector may differ from defaults if some buttons are omitted
// from config. Omitted buttons are ignored silently.
//
// Return value: std::vector<keybindings> (std::vector<std::unordered_set<int>>)
[[nodiscard]] std::vector<keybindings> primaryButtons(const Candidates&               candidates,
                                                      const std::vector<keybindings>& defaults);

// Maps a single button's keybindings from a list of candidates.
//
// Return value: keybindings (std::unordered_set<int>)
[[nodiscard]] keybindings primaryButton(const Candidates& candidates, const keybindings& defaults);

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
template<applet::Type TApplet>
[[nodiscard]] Keys keys(const toml::table& applet, const toml::table& global, const Keys& defaults);

} // namespace config::map

#include "Map.tpp"
