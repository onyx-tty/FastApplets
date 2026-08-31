// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Keybindings.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/Config/View/View.h"

#include <TomlQt/ArrayBounds.h>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

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

// TODO If applied key is already used elsewhere, the keys will behave unpredictably.
//      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
//      primary button 4 has missing keybinding, upon defaulting, primary button 4
//      will be set to Qt_Key4 and both buttons will then be set to Qt_Key4.
//      There should be a validation system in place for all keybindings, for example a
//      set with all keys which have already been exhausted.
namespace config::map::helpers {

using config::ConfigView;
using tomlqt::ArrayBounds;

[[nodiscard]] keybindings quit(const ConfigView& node, const keybindings& defaults);

// Expected format: array of arrays of strings
//
// Length of the vector may differ from defaults if some buttons are omitted
// from config. Omitted buttons are ignored silently.
[[nodiscard]] std::vector<keybindings> primaryButtons(
        const ConfigView& node, const std::vector<keybindings>& defaults);

// Expected format: array of strings
[[nodiscard]] keybindings primaryButton(const ConfigView& node, const keybindings& defaults);

} // namespace config::map::helpers
