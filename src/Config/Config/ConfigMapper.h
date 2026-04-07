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

#include "Config/Config/PowerApplet/PowerAppletConfig.h"
#include "Config/Config/Properties/EnvironmentProperties.h"
#include "Config/Config/Properties/LayoutProperties.h"
#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Config/Config/Properties/WindowProperties.h"
#include "Config/TOML/Types/NodePair.h"
#include "Config/TOML/Types/NodeView.h"

#include <cstddef>
#include <toml++/toml.hpp>
#include <QStringList>

class GlobalConfig;
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
        static void mapCommandArgument(node_view argument_node, PrimaryButtonData& button,
                                       const PrimaryButtonData* defaults, QStringList& arguments,
                                       size_t button_index, size_t arg_index,
                                       const QString& path_context);
        static void mapCommandArguments(node_view arguments_node, PrimaryButtonData& button,
                                        const PrimaryButtonData* defaults, QStringList& arguments,
                                        size_t button_index, const QString& path_context);
        static void mapCommand(node_view command_node, PrimaryButtonData& button,
                               const PrimaryButtonData* defaults, ShellCommand& command,
                               size_t button_index, const QString& path_context);
        static bool mapPrimaryButton(node_view                             button_data_node,
                                     std::vector<PrimaryButtonData>&       buttons,
                                     const std::vector<PrimaryButtonData>& default_buttons,
                                     const PrimaryButtonData* defaults, size_t button_index,
                                     const QString& path_context);
        static void mapPrimaryButtons(node_view                             primary_buttons_node,
                                      std::vector<PrimaryButtonData>&       primary_buttons,
                                      const std::vector<PrimaryButtonData>& defaults,
                                      const QString&                        path_context);
        static void mapLayout(node_view layout_node, LayoutProperties& layout,
                              const LayoutProperties& defaults, const QString& path_context);

        /* Environment Properties*/
        static void mapEnvironment(node_view environment_node, EnvironmentProperties& environment,
                                   const EnvironmentProperties& defaults,
                                   const QString&               path_context);

public:
        ConfigMapper() = delete;
        static void mapToGlobalConfig(const toml::table& config_table, GlobalConfig& config);
        static void mapToPowerAppletConfig(const toml::table& config_table,
                                           PowerAppletConfig& config);
};
