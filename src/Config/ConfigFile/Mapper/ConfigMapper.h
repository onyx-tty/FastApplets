// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Types/NodeView.h"

#include <toml++/toml.hpp>
#include <vector>

class GlobalConfig;
class NodePair;
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
//       as a fallback source but not mapped to any struct.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
class ConfigMapper final {
private:
        /* Window Properties */

        // Maps window from config sources.
        //
        // Fallback priority: power_applet.window -> global.window -> hardcoded defaults
        //
        // Expected format: window table containing title (string) and
        //                  size (array of two integers)
        //
        // Assigned value: WindowProperties
        static void mapWindow(NodePair nodes, WindowProperties& window,
                              const WindowProperties& defaults, const PathContext& path_context);

        /* Primary Button Properties */

        // Maps button from config sources.
        //
        // Fallback priority: power_applet.primary_button -> global.primary_button ->
        //                    hardcoded defaults
        //
        // Expected format: primary button table containing text_alignment (string),
        //                  icon_alignment (string), icon_size (array of two integers),
        //                  and policy (string)
        //
        // Assigned value: PrimaryButtonProperties
        static void mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                     const PrimaryButtonProperties& defaults,
                                     const PathContext&             path_context);

        /* Layout Properties */

        // Maps layout from a config source.
        //
        // Fallback priority: power_applet.layout -> hardcoded defaults
        //
        // Expected format: layout table containing primary_buttons (array of tables)
        //
        // Assigned value: LayoutProperties
        static void mapLayout(node_view layout_node, LayoutProperties& layout,
                              const LayoutProperties& defaults, const PathContext& path_context);

        // Maps primary_buttons from a config source.
        //
        // Aborts mapping if primary_buttons were defaulted by mapPrimaryButton, to respect
        // defaulting.
        // TODO: This should be done by mapPrimaryButtons instead, the helper
        //       should not be defaulting the entire vector, that's not its responsibility.
        //
        // Buttons with 'enabled = false' are silently omitted from output.
        //
        // Fallback priority: power_applet.layout.primary_buttons -> hardcoded defaults
        //
        // Expected format: primary_buttons (array of tables)
        //
        // Assigned value: vector of PowerButtonParams
        static void mapPrimaryButtons(node_view                             primary_buttons_node,
                                      std::vector<PowerButtonParams>&       primary_buttons,
                                      const std::vector<PowerButtonParams>& defaults,
                                      const PathContext&                    path_context);

        // Maps primary_button, including its attributes, from a config source.
        //
        // Because id, string, and order cannot be defaulted without confusing the
        // user, e.g. with disabled reboot it could lead to defaulting a suspend button
        // title as "Reboot", the method instead defaults all buttons and logs a warning.
        // When all buttons are defaulted, this method returns "true".
        // Otherwise "false".
        // TODO: id should be the only mandatory attribute, all other defaults can be
        //       deduced from it alone. The idea that all buttons should be defaulted
        //       with missing title/order is a leftover frm the previous index-based
        //       button passing.
        //
        // Fallback priority: power_applet.primary_buttons[index] -> hardcoded defaults
        //
        // Expected format: primary_buttons[index] table containing id (string),
        //                  text (string), order (int), command (string), and
        //                  enabled (bool)
        //
        // Assigned value: PowerButtonParams
        static bool mapPrimaryButton(node_view                             button_params_node,
                                     std::vector<PowerButtonParams>&       buttons,
                                     const std::vector<PowerButtonParams>& defaults,
                                     const PathContext&                    path_context);

        // Maps command from a config source.
        //
        // TODO: mapCommand defaults buttons without returning true to mapPrimaryButton,
        //       which results in button duplication, as mapping resumes after
        //       defaulting.
        //
        // Fallback priority: power_applet.primary_buttons[index].command ->
        //                    hardcoded defaults
        //
        // Expected format: primary_buttons[index].command (string)
        //
        // Assigned value: QString
        static void mapCommand(node_view command_node, std::vector<PowerButtonParams>& buttons,
                               const std::vector<PowerButtonParams>& defaults, QString& command,
                               const PathContext& path_context);

public:
        ConfigMapper() = delete;

        // Parses power_applet and global tables into PowerAppletConfig.
        //
        // Usage:
        //   PowerAppletConfig config;
        //   ConfigMapper::mapToPowerAppletConfig(power_applet_table, global_table, config);
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
        static void mapToPowerAppletConfig(const toml::table& power_applet_table,
                                           const toml::table& global_table, TConfig& config);
};

#include "ConfigMapper.tpp"
