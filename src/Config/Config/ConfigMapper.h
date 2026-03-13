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
        static void mapWindowSize(const toml::node_view<const toml::node>& size_node, QSize& size);
        static void mapWindowTitle(const toml::node_view<const toml::node>& title_node,
                                   QString&                                 title);
        static void mapWindowProperties(const toml::node_view<const toml::node>& window_node,
                                        Config::WindowProperties&                window);

        /* Button Properties */
        static void mapButtonTextAlignment(
                const toml::node_view<const toml::node>& text_alignment_node,
                Qt::Alignment&                           text_alignment);
        static void mapButtonIconAlignment(
                const toml::node_view<const toml::node>& icon_alignment_node,
                Qt::Alignment&                           icon_alignment);
        static void mapButtonIconSize(const toml::node_view<const toml::node>& icon_size_node,
                                      QSize&                                   icon_size);
        static void mapButtonPolicy(const toml::node_view<const toml::node>& policy_node,
                                    QSizePolicy&                             policy);
        static void mapButtonProperties(const toml::node_view<const toml::node>& button_node,
                                        Config::PrimaryButtonProperties&         button);

        /* Layout Properties */
        static void mapLayoutPrimaryButtonIdentifier(const toml::node_view<const toml::node> data,
                                                     PrimaryButtonData&                      button,
                                                     power_button_id& identifier,
                                                     size_t           button_index);
        static void mapLayoutPrimaryButtonText(const toml::node_view<const toml::node> data,
                                               PrimaryButtonData& button, QString& text,
                                               size_t button_index);
        static void mapLayoutPrimaryButtonOrder(const toml::node_view<const toml::node> data,
                                                PrimaryButtonData& button, long& order,
                                                std::vector<PrimaryButtonData>& buttons,
                                                size_t                          button_index);
        static void mapLayoutPrimaryButtonCommandProgram(
                const toml::node_view<const toml::node> data, PrimaryButtonData& button,
                QString& program, size_t button_index);
        static void mapLayoutPrimaryButtonCommandArgumentsArgument(
                const toml::node_view<const toml::node> data, PrimaryButtonData& button,
                QStringList& arguments, size_t button_index, size_t arg_index);
        static void mapLayoutPrimaryButtonCommandArguments(
                const toml::node_view<const toml::node> data, PrimaryButtonData& button,
                QStringList& arguments, size_t button_index);
        static void mapLayoutPrimaryButtonCommand(const toml::node_view<const toml::node> data,
                                                  PrimaryButtonData& button, ShellCommand& command,
                                                  size_t button_index);
        static void mapLayoutPrimaryButtonData(
                const toml::node_view<const toml::node>& button_data_node,
                PrimaryButtonData& button_data, std::vector<PrimaryButtonData>& buttons,
                size_t button_index);
        static void logButtonDisabled(const toml::node_view<const toml::node>& id_node,
                                      PrimaryButtonData& button_data, size_t button_index);
        static void mapLayoutPrimaryButtons(
                const toml::node_view<const toml::node>& primary_buttons_node,
                std::vector<PrimaryButtonData>&          primary_buttons);
        static void mapLayoutProperties(const toml::node_view<const toml::node>& layout_node,
                                        Config::WindowLayoutProperties&          layout);

        /* Environment Properties*/
        static void mapEnvironmentDBusMode(const toml::node_view<const toml::node>& dbus_mode_node,
                                           bool                                     dbus_mode);
        static void mapEnvironmentProperties(
                const toml::node_view<const toml::node>& environment_node,
                Config::EnvironmentProperties&           environment);

public:
        static void mapToConfig(const toml::table& config_table, Config& config);
};
