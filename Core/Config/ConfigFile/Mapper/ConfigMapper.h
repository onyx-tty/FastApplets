// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"

#include <optional>
#include <toml++/toml.hpp>
#include <vector>

namespace config::resolve {
class Candidates;
class PathContext;
} // namespace config::resolve

class LayoutProperties;
class PrimaryButtonParams;
class PrimaryButtonStyle;
class PrimaryButtonBehavior;
class WindowParams;
class QString;
class QSize;
class QSizePolicy;

namespace config {

// Maps TOML configuration to the config::schema::Config structure.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
class ConfigMapper final {
private:
        using Config      = config::schema::Config;
        using Candidates  = config::resolve::Candidates;
        using PathContext = config::resolve::PathContext;

        /* Helpers */

        template<typename T>
        [[nodiscard]] static T mapProperties(const Candidates& candidates, const T& defaults,
                                             const PathContext& path_context, auto fill_fn);

        /* WindowParams */

        // Maps WindowParams from config nodes.
        //
        // Fallback priority: applet.window_params -> global.window_params -> hardcoded defaults
        //
        // Expected format: window table containing title (string) and
        //                  size (array of two integers)
        //
        // Return value: WindowParams
        [[nodiscard]] static WindowParams windowParams(const Candidates&   candidates,
                                                       const WindowParams& defaults,
                                                       const PathContext&  path_context);

        /* PrimaryButtonStyle */

        // Maps PrimaryButtonStyle from config candidates.
        //
        // Fallback priority: applet.primary_button_style -> global.primary_button_style ->
        //                    hardcoded defaults
        //
        // Expected format: primary button table containing text_alignment (string),
        //                  icon_alignment (string), icon_size (array of two integers),
        //                  and policy (string)
        //
        // Return value: PrimaryButtonStyle
        [[nodiscard]] static PrimaryButtonStyle primaryButtonStyle(
                const Candidates& candidates, const PrimaryButtonStyle& defaults,
                const PathContext& path_context);

        /* PrimaryButtonBehavior */

        // Maps PrimaryButtonBehavior from config candidates.
        //
        // Fallback priority: applet.primary_button_behavior -> global.primary_button_behavior ->
        //                    hardcoded defaults
        //
        // Expected format: primary button table containing double_key_press (bool).
        //
        // Return value: PrimaryButtonBehavior
        [[nodiscard]] static PrimaryButtonBehavior primaryButtonBehavior(
                const Candidates& candidates, const PrimaryButtonBehavior& defaults,
                const PathContext& path_context);

        /* LayoutProperties */

        // Maps LayoutProperties from a config source.
        //
        // Fallback priority: applet.layout -> hardcoded defaults
        //
        // Return value: LayoutProperties
        template<applet::type TApplet>
        [[nodiscard]] static LayoutProperties layoutProperties(const Candidates&       candidates,
                                                               const LayoutProperties& defaults,
                                                               const PathContext& path_context);

        // Maps std::vector<PrimaryButtonParams> from a config source.
        //
        // applet::type must be specified due to differences in type enums.
        //
        // Regarding PrimaryButtonParams::command: QProcess::splitCommand() does not
        // interpret single quotes as quotes, but as regular characters. This function
        // converts single quotes to double quotes to work around that limitation.
        //
        // Defaults the buttons if none are found.
        //
        // Fallback priority: applet.layout.primary_buttons -> hardcoded defaults
        //
        // Return value: std::vector<PrimaryButtonParams>
        template<applet::type TApplet>
        [[nodiscard]] static std::vector<PrimaryButtonParams> primaryButtonParams(
                const Candidates& candidates, const std::vector<PrimaryButtonParams>& defaults,
                const PathContext& path_context);

        // Maps std::optional<PrimaryButtonParams>, including its attributes, from a config source.
        //
        // Buttons with invalid type are omitted with a warning.
        //
        // Fallback priority: applet.primary_buttons[index] -> hardcoded defaults
        //
        // Expected format: primary_buttons[index] table containing type (string),
        //                  text (string), command (string)
        //
        // Return value: std::optional<PrimaryButtonParams>
        template<applet::type TApplet>
        [[nodiscard]] static std::optional<PrimaryButtonParams> primaryButtonParams(
                const Candidates& candidates, const PathContext& path_context);

public:
        ConfigMapper() = delete;

        // Parses applet and global tables into config::schema::Config.
        //
        // Usage:
        //   auto config = ConfigMapper::config<applet::type::x>(applet, global, defaults);
        //
        // The applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling (initialized in main()).
        //
        // Return value: config::schema::Config
        template<applet::type TApplet>
        [[nodiscard]] static Config config(const toml::table& applet, const toml::table& global,
                                           const Config& defaults);
};

} // namespace config

#include "ConfigMapper.tpp"
