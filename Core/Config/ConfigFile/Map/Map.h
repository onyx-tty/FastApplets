// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Schema/Config.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"

#include <optional>
#include <toml++/toml.hpp>
#include <vector>

namespace config::resolve {
class Candidates;
class PathContext;
} // namespace config::resolve

class PrimaryButtonParams;
class PerPrimaryButtonParams;
class PrimaryButtonStyle;
class PrimaryButtonBehavior;
class WindowParams;
class QString;
class QSize;
class QSizePolicy;

// Maps TOML configuration to the config::schema::Config structure.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
namespace config::map {

using Config      = config::schema::Config;
using Candidates  = config::resolve::Candidates;
using PathContext = config::resolve::PathContext;

/* Helpers */

template<typename T>
[[nodiscard]] T properties(const Candidates& candidates, const T& defaults,
                           const PathContext& path_context, auto fill_fn);

/* WindowParams */

// Maps WindowParams from config nodes.
//
// Fallback priority: applet > global > hardcoded defaults
//
// Expected format: window table containing title (string) and
//                  size (array of two integers)
//
// Return value: WindowParams
[[nodiscard]] WindowParams windowParams(const Candidates& candidates, const WindowParams& defaults,
                                        const PathContext& path_context);

/* PrimaryButtonParams */

// Maps PrimaryButtonParams from a list of candidates.
//
// Expected format: primary_button table containing double_key_press (bool),
//                  text_alignment (string), icon_alignment (string),
//                  icon_size (array of two integers), policy (string), and
//                  list (array of tables)
//
// Return value: PrimaryButtonParams
template<applet::type TApplet>
[[nodiscard]] PrimaryButtonParams primaryButtonParams(const Candidates&          candidates,
                                                      const PrimaryButtonParams& defaults,
                                                      const PathContext&         path_context);

// Maps std::vector<PerPrimaryButtonParams> from a list of candidates.
//
// Expected format: array of tables
//
// Return value: std::vector<PerPrimaryButtonParams>
template<applet::type TApplet>
[[nodiscard]] std::vector<PerPrimaryButtonParams> perPrimaryButtonParamsList(
        const Candidates& candidates, const std::vector<PerPrimaryButtonParams>& defaults,
        const PathContext& path_context);

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
template<applet::type TApplet>
[[nodiscard]] std::optional<PerPrimaryButtonParams> perPrimaryButtonParams(
        const Candidates& candidates, const PathContext& path_context);

// Maps PrimaryButtonStyle from a list of candidates.
//
// Expected format: primary_button table containing text_alignment (string),
//                  icon_alignment (string), icon_size (array of two integers),
//                  and policy (string)
//
// Return value: PrimaryButtonStyle
[[nodiscard]] PrimaryButtonStyle primaryButtonStyle(const Candidates&         candidates,
                                                    const PrimaryButtonStyle& defaults,
                                                    const PathContext&        path_context);

// Maps PrimaryButtonBehavior from a list of candidates.
//
// Expected format: primary button table containing double_key_press (bool).
//
// Return value: PrimaryButtonBehavior
[[nodiscard]] PrimaryButtonBehavior primaryButtonBehavior(const Candidates&            candidates,
                                                          const PrimaryButtonBehavior& defaults,
                                                          const PathContext& path_context);

// Parses applet and global tables into config::schema::Config.
//
// Usage:
//   auto config = ConfigMapper::config<applet::type::foo>(applet, global, defaults);
//
// The applet table supplies primary configuration and overrides, global
// provides fallbacks.
//
// QApplication must exist before calling.
//
// Return value: config::schema::Config
template<applet::type TApplet>
[[nodiscard]] Config config(const toml::table& applet, const toml::table& global,
                            const Config& defaults);

} // namespace config::map

#include "Map.tpp"
