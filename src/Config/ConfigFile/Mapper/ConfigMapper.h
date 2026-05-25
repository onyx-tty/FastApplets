// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/Types/ResolverCandidate.h"
#include "Config/Types/NodeView.h"

#include <initializer_list>
#include <optional>
#include <toml++/toml.hpp>
#include <vector>

class GlobalConfig;
class PathContext;
class PowerButtonParams;
class PrimaryButtonProperties;
class WindowProperties;
class LayoutProperties;
class QString;
class QSize;
class QSizePolicy;

// Maps TOML configuration to PowerAppletConfig structure.
//
// Note: This file only handles PowerAppletConfig mapping. GlobalConfig is only used
//       as a fallback candidate but not mapped to any struct.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
class ConfigMapper final {
private:
        /* Window Properties */

        // Maps window from config nodes.
        //
        // Fallback priority: power_applet.window -> global.window -> hardcoded defaults
        //
        // Expected format: window table containing title (string) and
        //                  size (array of two integers)
        //
        // Assigned value: WindowProperties
        static WindowProperties window(std::initializer_list<ResolverCandidate> candidates,
                                       const WindowProperties&                  defaults,
                                       const PathContext&                       path_context);

        /* Primary Button Properties */

        // Maps button from config candidates.
        //
        // Fallback priority: power_applet.primary_button -> global.primary_button ->
        //                    hardcoded defaults
        //
        // Expected format: primary button table containing text_alignment (string),
        //                  icon_alignment (string), icon_size (array of two integers),
        //                  and policy (string)
        //
        // Assigned value: PrimaryButtonProperties
        static PrimaryButtonProperties primaryButton(
                std::initializer_list<ResolverCandidate> candidates,
                const PrimaryButtonProperties& defaults, const PathContext& path_context);

        /* Layout Properties */

        // Maps layout from a config source.
        //
        // Fallback priority: power_applet.layout -> hardcoded defaults
        //
        // Expected format: layout table containing primary_buttons (array of tables)
        //
        // Assigned value: LayoutProperties
        static LayoutProperties layout(node_view node, const LayoutProperties& defaults,
                                       const PathContext& path_context);

        // Maps primary_buttons from a config source.
        //
        // Defaults the buttons if none are found.
        //
        // Fallback priority: power_applet.layout.primary_buttons -> hardcoded defaults
        //
        // Expected format: primary_buttons (array of tables)
        //
        // Assigned value: vector of PowerButtonParams
        static std::vector<PowerButtonParams> primaryButtons(
                node_view node, const std::vector<PowerButtonParams>& defaults,
                const PathContext& path_context);

        // Maps primary_button, including its attributes, from a config source.
        //
        // Buttons with invalid type are omitted with a warning.
        //
        // Fallback priority: power_applet.primary_buttons[index] -> hardcoded defaults
        //
        // Expected format: primary_buttons[index] table containing type (string),
        //                  text (string), command (string)
        //
        // Assigned value: PowerButtonParams
        static std::optional<PowerButtonParams> primaryButton(node_view          node,
                                                              const PathContext& path_context);

public:
        ConfigMapper() = delete;

        // Parses power_applet and global tables into PowerAppletConfig.
        //
        // Usage:
        //   PowerAppletConfig config;
        //   ConfigMapper::config(power_applet_table, global_table, config);
        //
        // The power_applet table supplies primary configuration and overrides, global
        // provides fallbacks.
        //
        // QApplication must exist before calling (initialized in main()).
        //
        // Only PowerAppletConfig is supported. Template arg TConfig is used to break a
        // circular dependency between this header and PowerAppletConfig.h by deferring
        // the instantiation of PowerAppletConfig.
        template<typename TConfig>
        static TConfig config(const toml::table& power_applet, const toml::table& global);
};

#include "ConfigMapper.tpp"
