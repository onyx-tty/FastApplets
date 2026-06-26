// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"

#include <optional>
#include <toml++/toml.hpp>
#include <vector>

class GlobalConfig;
class PathContext;
class ResolverCandidates;
class PrimaryButtonParams;
class PrimaryButtonProperties;
class WindowProperties;
class QString;
class QSize;
class QSizePolicy;

// Maps TOML configuration to XAppletConfig structure.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
class ConfigMapper final {
private:
        /* Helpers */

        template<typename T>
        static T mapProperties(const ResolverCandidates& candidates, const T& defaults,
                               const PathContext& path_context, auto fill_fn);

        /* Window Properties */

        // Maps window from config nodes.
        //
        // Fallback priority: applet.window -> global.window -> hardcoded defaults
        //
        // Expected format: window table containing title (string) and
        //                  size (array of two integers)
        //
        // Return value: WindowProperties
        static WindowProperties window(const ResolverCandidates& candidates,
                                       const WindowProperties&   defaults,
                                       const PathContext&        path_context);

        /* Primary Button Properties */

        // Maps button from config candidates.
        //
        // Fallback priority: applet.primary_button -> global.primary_button ->
        //                    hardcoded defaults
        //
        // Expected format: primary button table containing text_alignment (string),
        //                  icon_alignment (string), icon_size (array of two integers),
        //                  and policy (string)
        //
        // Return value: PrimaryButtonProperties
        static PrimaryButtonProperties primaryButton(const ResolverCandidates&      candidates,
                                                     const PrimaryButtonProperties& defaults,
                                                     const PathContext&             path_context);

        /* Layout Properties */

        // Maps layout from a config source.
        //
        // applet::type must be specified due to differing TPrimaryButtonParams.
        //
        // Fallback priority: applet.layout -> hardcoded defaults
        //
        // Return value: LayoutProperties<TPrimaryButtonParams>
        template<applet::type TApplet>
        static LayoutProperties<typename AppletTraits<TApplet>::TPrimaryButtonParams> layout(
                const ResolverCandidates& candidates,
                const LayoutProperties<typename AppletTraits<TApplet>::TPrimaryButtonParams>&
                                   defaults,
                const PathContext& path_context);

        // Maps primary_buttons from a config source.
        //
        // applet::type must be specified due to differing TPrimaryButtonParams.
        //
        // Defaults the buttons if none are found.
        //
        // Fallback priority: applet.layout.primary_buttons -> hardcoded defaults
        //
        // Return value: std::vector<TPrimaryButtonParams>
        template<applet::type TApplet>
        static std::vector<typename AppletTraits<TApplet>::TPrimaryButtonParams> primaryButtons(
                const ResolverCandidates&                                                candidates,
                const std::vector<typename AppletTraits<TApplet>::TPrimaryButtonParams>& defaults,
                const PathContext& path_context);

        // Maps primary_button, including its attributes, from a config source.
        //
        // applet::type must be specified due to differing TPrimaryButtonParams.
        //
        // Buttons with invalid type are omitted with a warning.
        //
        // Fallback priority: applet.primary_buttons[index] -> hardcoded defaults
        //
        // Expected format: primary_buttons[index] table containing type (string),
        //                  text (string), command (string)
        //
        // Return value: std::optional<TPrimaryButtonParams>
        template<applet::type TApplet>
        static std::optional<typename AppletTraits<TApplet>::TPrimaryButtonParams> primaryButton(
                const ResolverCandidates& candidates, const PathContext& path_context);

public:
        ConfigMapper() = delete;

        // Parses applet and global tables into XAppletConfig.
        //
        // applet::type must be specified due to differing LayoutProperties.
        //
        // Usage:
        //   XAppletConfig config = ConfigMapper::config<applet::type::x>(applet, global, defaults);
        //
        // The applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling (initialized in main()).
        //
        // Return value: TConfig
        template<applet::type TApplet>
        static AppletTraits<TApplet>::TConfig config(const toml::table& applet,
                                                     const toml::table& global,
                                                     const AppletTraits<TApplet>::TConfig& defaults);
};

#include "ConfigMapper.tpp"
