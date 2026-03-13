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

#include "Config.h"
#include "Config/TOML/NodeView.h"
#include "UI/Enums/ButtonIDs.h"

#include <toml++/toml.hpp>
#include <QStringList>

class QString;
class QSize;
class QSizePolicy;
struct ShellCommand;

class ConfigMapper final {
private:
        ConfigMapper() = delete;

        /* Window Properties */
        static void mapWindowSize(node_view size_node, QSize& size);
        static void mapWindowTitle(node_view title_node, QString& title);
        static void mapWindowProperties(node_view window_node, Config::WindowProperties& window);

        /* Button Properties */
        static void mapButtonTextAlignment(node_view      text_alignment_node,
                                           Qt::Alignment& text_alignment);
        static void mapButtonIconAlignment(node_view      icon_alignment_node,
                                           Qt::Alignment& icon_alignment);
        static void mapButtonIconSize(node_view icon_size_node, QSize& icon_size);
        static void mapButtonPolicy(node_view policy_node, QSizePolicy& policy);
        static void mapButtonProperties(node_view                        button_node,
                                        Config::PrimaryButtonProperties& button);

        /* Layout Properties */
        static void mapLayoutPrimaryButtonIdentifier(node_view          button_node,
                                                     PrimaryButtonData& button,
                                                     power_button_id&   identifier,
                                                     size_t             button_index);
        static void mapLayoutPrimaryButtonText(node_view text_node, PrimaryButtonData& button,
                                               QString& text, size_t button_index);
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
        static void logButtonDisabled(node_view identifier_node, PrimaryButtonData& button_data,
                                      size_t button_index);
        static void mapLayoutPrimaryButtons(node_view                       buttons_node,
                                            std::vector<PrimaryButtonData>& buttons);
        static void mapLayoutProperties(node_view                       layout_node,
                                        Config::WindowLayoutProperties& layout);

        /* Environment Properties*/
        static void mapEnvironmentDBusMode(node_view dbus_mode_node, bool dbus_mode);
        static void mapEnvironmentProperties(node_view                      environment_node,
                                             Config::EnvironmentProperties& environment);

public:
        static void mapToConfig(const toml::table& config_table, Config& config);
};
