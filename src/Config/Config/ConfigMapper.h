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
#include "Config/TOML/NodeView.h"
#include "UI/Enums/ButtonIDs.h"

#include <array>
#include <toml++/toml.hpp>
#include <QStringList>

class GlobalConfig;
class QString;
class QSize;
class QSizePolicy;
struct ShellCommand;

struct NodePair final {
        node_view primary;
        node_view fallback;
};

class ConfigMapper final {
private:
        ConfigMapper() = delete;

        /* Window Properties */
        static void mapWindowSize(NodePair nodes, QSize& size, const QString& path_context);
        static void mapWindowTitle(NodePair nodes, QString& title, const QString& path_context);
        static void mapWindowProperties(NodePair nodes, WindowProperties& window,
                                        const QString& path_context);

        /* Primary Button Properties */
        static void mapPrimaryButtonTextAlignment(NodePair nodes, Qt::Alignment& text_alignment,
                                                  const QString& path_context);
        static void mapPrimaryButtonIconAlignment(NodePair nodes, Qt::Alignment& icon_alignment,
                                                  const QString& path_context);
        static void mapPrimaryButtonIconSize(NodePair nodes, QSize& icon_size,
                                             const QString& path_context);
        static void mapPrimaryButtonPolicy(NodePair nodes, QSizePolicy& policy,
                                           const QString& path_context);
        static void mapPrimaryButtonProperties(NodePair nodes, PrimaryButtonProperties& button,
                                               const QString& path_context);

        /* Layout Properties */
        static void mapLayoutPrimaryButtonID(node_view id_node, PrimaryButtonData& button,
                                             power_button_id& id, size_t button_index,
                                             const QString& path_context);
        static void mapLayoutPrimaryButtonLabel(node_view label_node, PrimaryButtonData& button,
                                                QString& label, size_t button_index,
                                                const QString& path_context);
        static void mapLayoutPrimaryButtonOrder(node_view order_node, PrimaryButtonData& button,
                                                long&                           order,
                                                std::vector<PrimaryButtonData>& buttons,
                                                size_t button_index, const QString& path_context);
        static void mapLayoutPrimaryButtonCommandProgram(node_view          program_node,
                                                         PrimaryButtonData& button,
                                                         QString& program, size_t button_index,
                                                         const QString& path_context);
        static void mapLayoutPrimaryButtonCommandArgumentsArgument(
                node_view argument_node, PrimaryButtonData& button, QStringList& arguments,
                size_t button_index, size_t arg_index, const QString& path_context);
        static void mapLayoutPrimaryButtonCommandArguments(node_view          arguments_node,
                                                           PrimaryButtonData& button,
                                                           QStringList&       arguments,
                                                           size_t             button_index,
                                                           const QString&     path_context);
        static void mapLayoutPrimaryButtonCommand(node_view command_node, PrimaryButtonData& button,
                                                  ShellCommand& command, size_t button_index,
                                                  const QString& path_context);
        static bool mapLayoutPrimaryButtonData(node_view                       button_data_node,
                                               std::vector<PrimaryButtonData>& buttons,
                                               size_t button_index, const QString& path_context);
        static void mapLayoutPrimaryButtons(node_view                       primary_buttons_node,
                                            std::vector<PrimaryButtonData>& primary_buttons,
                                            const QString&                  path_context);
        static void mapLayoutProperties(node_view layout_node, LayoutProperties& layout,
                                        const QString& path_context);

        /* Environment Properties*/
        static void mapEnvironmentDBusMode(node_view dbus_mode_node, bool& dbus_mode,
                                           const QString& path_context);
        static void mapEnvironmentProperties(node_view              environment_node,
                                             EnvironmentProperties& environment,
                                             const QString&         path_context);

public:
        static void mapToGlobalConfig(const toml::table& config_table, GlobalConfig& config);
        static void mapToPowerAppletConfig(const toml::table& config_table,
                                           PowerAppletConfig& config);
};
