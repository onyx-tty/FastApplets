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

#include "UI/Enums/ButtonIDs.h"

#include <toml++/toml.hpp>
#include <QStringList>

class Config;
class PrimaryButtonData;
class QString;
struct ShellCommand;

class ConfigMapper final {
private:
        ConfigMapper() = delete;

        /* Window Properties */
        static void mapWindowSize(const toml::table& window, Config& config);
        static void mapWindowTitle(const toml::table& window, Config& config);
        static void mapWindowProperties(const toml::table& config_table, Config& config);

        /* Button Properties */
        static void mapButtonTextAlignment(const toml::table& button, Config& config);
        static void mapButtonIconAlignment(const toml::table& button, Config& config);
        static void mapButtonIconSize(const toml::table& button, Config& config);
        static void mapButtonPolicy(const toml::table& button, Config& config);
        static void mapButtonProperties(const toml::table& config_table, Config& config);

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
                                                size_t button_index);
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
                                                  PrimaryButtonData& button, size_t button_index);
        static void mapLayoutPrimaryButtonData(const toml::table& button_table,
                                               PrimaryButtonData& button_data, size_t button_index);
        static void logButtonDisabled(const toml::table& button_table,
                                      PrimaryButtonData& button_data, size_t button_index);
        static void mapLayoutPrimaryButtons(const toml::table& layout, Config& config);
        static void mapLayoutProperties(const toml::table& config_table, Config& config);

        /* Environment Properties*/
        static void mapEnvironmentDBusMode(const toml::table& environment, Config& config);
        static void mapEnvironmentProperties(const toml::table& config_table, Config& config);

public:
        static void mapToConfig(const toml::table& config_table, Config& config);
};
