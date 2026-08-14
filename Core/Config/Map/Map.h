// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/ArraySpec.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

namespace config {
class Candidate;
class PathContext;
} // namespace config

class PrimaryButtonParams;
class PerPrimaryButtonParams;
class PrimaryButtonStyle;
class PrimaryButtonBehavior;
class WindowParams;
class QString;
class QSize;
class QSizePolicy;

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

// Maps TOML configuration to the schemas.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
namespace config::map {

using config::ArraySpec;
using config::CandidateIndex;
using config::Candidates;
using config::PathContext;
using config::schema::Config;
using config::schema::Keys;

/* Helpers */

template<typename T>
[[nodiscard]] std::optional<T> table(const Candidates& candidates, auto fill_fn);

/* WindowParams */

// Maps WindowParams from config nodes.
//
// Fallback priority: applet > global > hardcoded defaults
//
// Expected format: window table containing title (string) and
//                  size (array of two integers)
//
// Return value: WindowParams
[[nodiscard]] WindowParams windowParams(const Candidates& candidates, const WindowParams& defaults);

/* PrimaryButtonParams */

// Maps PrimaryButtonParams from a list of candidates.
//
// Expected format: primary_button table containing double_key_press (bool),
//                  text_alignment (string), icon_alignment (string),
//                  icon_size (array of two integers), policy (string), and
//                  list (array of tables)
//
// Return value: PrimaryButtonParams
template<applet::Type TApplet>
[[nodiscard]] PrimaryButtonParams primaryButtonParams(const Candidates&          candidates,
                                                      const PrimaryButtonParams& defaults);

// Maps std::vector<PerPrimaryButtonParams> from a list of candidates.
//
// Expected format: array of tables
//
// Return value: std::vector<PerPrimaryButtonParams>
template<applet::Type TApplet>
[[nodiscard]] std::vector<PerPrimaryButtonParams> perPrimaryButtonParamsList(
        const Candidates& candidates, const std::vector<PerPrimaryButtonParams>& defaults);

// Maps PerPrimaryButtonParams from a list of candidates.
//
// Expected format: primary_buttons.list[index] table containing type (string),
//                  text (string), and command (string)
//
// Regarding PerPrimaryButtonParams::command: QProcess::splitCommand() does not
// interpret single quotes as quotes, but as regular characters. This function
// converts single quotes to double quotes to work around that limitation.
//
// Return value: std::optional<PerPrimaryButtonParams>
template<applet::Type TApplet>
[[nodiscard]] std::optional<PerPrimaryButtonParams> perPrimaryButtonParams(
        const Candidates& candidates);

// Maps PrimaryButtonStyle from a list of candidates.
//
// Expected format: primary_button table containing text_alignment (string),
//                  icon_alignment (string), icon_size (array of two integers),
//                  and policy (string)
//
// Return value: PrimaryButtonStyle
[[nodiscard]] PrimaryButtonStyle primaryButtonStyle(const Candidates&         candidates,
                                                    const PrimaryButtonStyle& defaults);

// Maps PrimaryButtonBehavior from a list of candidates.
//
// Expected format: primary button table containing double_key_press (bool).
//
// Return value: PrimaryButtonBehavior
[[nodiscard]] PrimaryButtonBehavior primaryButtonBehavior(const Candidates&            candidates,
                                                          const PrimaryButtonBehavior& defaults);

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

// Parses applet and global tables into config::schema::Config.
//
// Usage:
//   auto config = ConfigMapper::config<applet::Type::foo>(applet, global, defaults);
//
// The applet table supplies primary configuration and overrides, global
// provides fallbacks.
//
// QApplication must exist before calling.
//
// Return value: config::schema::Config
template<applet::Type TApplet>
[[nodiscard]] Config config(const toml::table& applet, const toml::table& global,
                            const Config& defaults);

} // namespace config::map

#include "Map.tpp"
