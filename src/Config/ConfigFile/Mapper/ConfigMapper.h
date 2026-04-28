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

#include "Config/Resolvers/Resolvers.h"
#include "Config/TOML/Types/NodePair.h"
#include "Config/TOML/Types/NodeView.h"

#include <cstddef>
#include <toml++/toml.hpp>
#include <QStringList>

class EnvironmentProperties;
class LayoutProperties;
class WindowProperties;
class GlobalConfig;
class PowerButtonParams;
class PrimaryButtonProperties;
class QString;
class QSize;
class QSizePolicy;
class ShellCommand;

class ConfigMapper final {
private:
        /* Window Properties */
        static void mapWindow(NodePair nodes, WindowProperties& window,
                              const WindowProperties& defaults, const PathContext& path_context);

        /* Primary Button Properties */
        static void mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                     const PrimaryButtonProperties& defaults,
                                     const PathContext&             path_context);

        /* Layout Properties */
        static void mapLayout(node_view layout_node, LayoutProperties& layout,
                              const LayoutProperties& defaults, const PathContext& path_context);
        static void mapPrimaryButtons(node_view                             primary_buttons_node,
                                      std::vector<PowerButtonParams>&       primary_buttons,
                                      const std::vector<PowerButtonParams>& defaults,
                                      const PathContext&                    path_context);
        static bool mapPrimaryButton(node_view                             button_params_node,
                                     std::vector<PowerButtonParams>&       buttons,
                                     const std::vector<PowerButtonParams>& defaults,
                                     const PathContext&                    path_context);
        static void mapCommand(node_view command_node, std::vector<PowerButtonParams>& buttons,
                               const std::vector<PowerButtonParams>& defaults,
                               ShellCommand& command, const PathContext& path_context);
        static void mapCommandArguments(node_view                             arguments_node,
                                        std::vector<PowerButtonParams>&       buttons,
                                        const std::vector<PowerButtonParams>& defaults,
                                        QStringList& arguments, const PathContext& path_context);
        static void mapCommandArgument(node_view                             argument_node,
                                       std::vector<PowerButtonParams>&       buttons,
                                       const std::vector<PowerButtonParams>& defaults,
                                       QStringList& arguments, const PathContext& path_context);

        /* Environment Properties*/
        static void mapEnvironment(node_view environment_node, EnvironmentProperties& environment,
                                   const EnvironmentProperties& defaults,
                                   const PathContext&           path_context);

public:
        ConfigMapper() = delete;
        template<typename TConfig>
        static void mapToPowerAppletConfig(const toml::table& power_applet_table,
                                           const toml::table& global_table, TConfig& config);
};

#include "ConfigMapper.tpp"
