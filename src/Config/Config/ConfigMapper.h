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

#include <toml++/toml.hpp>
#include <QStringList>

class GlobalConfig;
class QString;
class QSize;
class QSizePolicy;
struct ShellCommand;

class ConfigMapper final {
private:
        ConfigMapper() = delete;

        /* Window Properties */
        static void mapWindowSize(node_view size_node, node_view global_fallback_node, QSize& size);
        static void mapWindowTitle(node_view title_node, node_view global_fallback_node,
                                   QString& title);
        static void mapWindowProperties(node_view window_node, node_view global_fallback_node,
                                        WindowProperties& window);

        /* Primary Button Properties */
        static void mapPrimaryButtonTextAlignment(node_view      text_alignment_node,
                                                  node_view      global_fallback_node,
                                                  Qt::Alignment& text_alignment);
        static void mapPrimaryButtonIconAlignment(node_view      icon_alignment_node,
                                                  node_view      global_fallback_node,
                                                  Qt::Alignment& icon_alignment);
        static void mapPrimaryButtonIconSize(node_view icon_size_node,
                                             node_view global_fallback_node, QSize& icon_size);
        static void mapPrimaryButtonPolicy(node_view policy_node, node_view global_fallback_node,
                                           QSizePolicy& policy);
        static void mapPrimaryButtonProperties(node_view                button_node,
                                               node_view                global_fallback_node,
                                               PrimaryButtonProperties& button);

        /* Layout Properties */
        static void mapLayoutPrimaryButtonID(node_view id_node, PrimaryButtonData& button,
                                             power_button_id& id, size_t button_index);
        static void mapLayoutPrimaryButtonLabel(node_view label_node, PrimaryButtonData& button,
                                                QString& label, size_t button_index);
        static void mapLayoutPrimaryButtonOrder(node_view order_node, PrimaryButtonData& button,
                                                long&                           order,
                                                std::vector<PrimaryButtonData>& buttons,
                                                size_t                          button_index);
        static void mapLayoutPrimaryButtonCommandProgram(node_view          program_node,
                                                         PrimaryButtonData& button,
                                                         QString& program, size_t button_index);
        static void mapLayoutPrimaryButtonCommandArgumentsArgument(node_view          argument_node,
                                                                   PrimaryButtonData& button,
                                                                   QStringList&       arguments,
                                                                   size_t             button_index,
                                                                   size_t             arg_index);
        static void mapLayoutPrimaryButtonCommandArguments(node_view          arguments_node,
                                                           PrimaryButtonData& button,
                                                           QStringList&       arguments,
                                                           size_t             button_index);
        static void mapLayoutPrimaryButtonCommand(node_view command_node, PrimaryButtonData& button,
                                                  ShellCommand& command, size_t button_index);
        static void mapLayoutPrimaryButtonData(node_view                       button_data_node,
                                               PrimaryButtonData&              button_data,
                                               std::vector<PrimaryButtonData>& buttons,
                                               size_t                          button_index);
        static void logButtonDisabled(node_view id_node, PrimaryButtonData& button_data,
                                      size_t button_index);
        static void mapLayoutPrimaryButtons(node_view                       buttons_node,
                                            std::vector<PrimaryButtonData>& buttons);
        static void mapLayoutProperties(node_view layout_node, LayoutProperties& layout);

        /* Environment Properties*/
        static void mapEnvironmentDBusMode(node_view dbus_mode_node, bool& dbus_mode);
        static void mapEnvironmentProperties(node_view              environment_node,
                                             EnvironmentProperties& environment);

public:
        static void mapToGlobalConfig(const toml::table& config_table, GlobalConfig& config);
        static void mapToPowerAppletConfig(const toml::table& config_table,
                                           PowerAppletConfig& config);
};
