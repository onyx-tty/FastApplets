/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/TOML/Types/NodePair.h"
#include "Config/TOML/Types/NodeView.h"

#include <cstddef>
#include <toml++/toml.hpp>
#include <QStringList>

class GlobalConfig;
class PowerButtonParams;
class QString;
class QSize;
class QSizePolicy;
struct ShellCommand;

class ConfigMapper final {
private:
        /* Window Properties */
        static void mapWindow(NodePair nodes, WindowProperties& window,
                              const WindowProperties& defaults, const QString& path_context);

        /* Primary Button Properties */
        static void mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                     const PrimaryButtonProperties& defaults,
                                     const QString&                 path_context);

        /* Layout Properties */
        static void mapCommandArgument(node_view argument_node, PowerButtonParams& button,
                                       const PowerButtonParams* defaults, QStringList& arguments,
                                       const QString& path_context);
        static void mapCommandArguments(node_view arguments_node, PowerButtonParams& button,
                                        const PowerButtonParams* defaults, QStringList& arguments,
                                        const QString& path_context);
        static void mapCommand(node_view command_node, PowerButtonParams& button,
                               const PowerButtonParams* defaults, ShellCommand& command,
                               const QString& path_context);
        static bool mapPrimaryButton(node_view                             button_params_node,
                                     std::vector<PowerButtonParams>&       buttons,
                                     const std::vector<PowerButtonParams>& default_buttons,
                                     const PowerButtonParams*              defaults,
                                     const QString&                        path_context);
        static void mapPrimaryButtons(node_view                             primary_buttons_node,
                                      std::vector<PowerButtonParams>&       primary_buttons,
                                      const std::vector<PowerButtonParams>& defaults,
                                      const QString&                        path_context);
        static void mapLayout(node_view layout_node, LayoutProperties& layout,
                              const LayoutProperties& defaults, const QString& path_context);

        /* Environment Properties*/
        static void mapEnvironment(node_view environment_node, EnvironmentProperties& environment,
                                   const EnvironmentProperties& defaults,
                                   const QString&               path_context);

public:
        ConfigMapper() = delete;
        static void mapToPowerAppletConfig(const toml::table& power_applet_table,
                                           const toml::table& global_table,
                                           PowerAppletConfig& config);
};
