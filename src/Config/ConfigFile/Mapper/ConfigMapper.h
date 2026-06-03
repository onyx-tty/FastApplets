// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/ConfigFile/Properties/LayoutProperties.h"

#include <optional>
#include <toml++/toml.hpp>
#include <vector>

class GlobalConfig;
class PathContext;
class ResolverCandidates;
class PowerButtonParams;
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
        // Fallback priority: applet.layout -> hardcoded defaults
        //
        // Expected format: layout table containing primary_buttons (array of tables)
        //
        // Return value: LayoutProperties
        static LayoutProperties<PowerButtonParams> layout(
                const ResolverCandidates&                  candidates,
                const LayoutProperties<PowerButtonParams>& defaults,
                const PathContext&                         path_context);

        // Maps primary_buttons from a config source.
        //
        // Defaults the buttons if none are found.
        //
        // Fallback priority: applet.layout.primary_buttons -> hardcoded defaults
        //
        // Expected format: primary_buttons (array of tables)
        //
        // Return value: std::vector<PowerButtonParams>
        static std::vector<PowerButtonParams> primaryButtons(
                const ResolverCandidates&             candidates,
                const std::vector<PowerButtonParams>& defaults, const PathContext& path_context);

        // Maps primary_button, including its attributes, from a config source.
        //
        // Buttons with invalid type are omitted with a warning.
        //
        // Fallback priority: applet.primary_buttons[index] -> hardcoded defaults
        //
        // Expected format: primary_buttons[index] table containing type (string),
        //                  text (string), command (string)
        //
        // Return value: std::optional<PowerButtonParams>
        static std::optional<PowerButtonParams> primaryButton(const ResolverCandidates& candidates,
                                                              const PathContext& path_context);

public:
        ConfigMapper() = delete;

        // Parses applet and global tables into XAppletConfig.
        //
        // Usage:
        //   XAppletConfig config = ConfigMapper::config(applet, global, defaults);
        //
        // The applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling (initialized in main()).
        //
        // Return value: TConfig
        template<typename TConfig>
        static TConfig config(const toml::table& applet, const toml::table& global,
                              const TConfig& defaults);
};

#include "ConfigMapper.tpp"
