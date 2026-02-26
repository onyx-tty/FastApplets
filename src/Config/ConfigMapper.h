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

class Config;
class Keys;
class PrimaryButtonData;

#include <toml++/toml.hpp>

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
        static void mapLayoutPrimaryButtonData(const toml::table& button_table,
                                               PrimaryButtonData& button_data, size_t button_index);
        static void logButtonDisabled(const toml::table& button_table,
                                      PrimaryButtonData& button_data, size_t button_index);
        static void mapLayoutPrimaryButtons(const toml::table& layout, Config& config);
        static void mapLayoutProperties(const toml::table& config_table, Config& config);

        /* Global Keys */
        static void mapGlobalQuitKeys(const toml::table& global, Keys& keys);
        static void mapGlobalKeys(const toml::table& keys_table, Keys& keys);

        /* Power Applet Keys*/
        static void mapPowerAppletQuitKeys(const toml::table& power_applet, Keys& keys);
        static void mapPowerAppletPrimaryButtonKeys(const toml::table& power_applet, Keys& keys);
        static void mapPowerAppletKeys(const toml::table& keys_table, Keys& keys);

public:
        /* Main functions */
        static void mapToConfig(const toml::table& config_table, Config& config);
        static void mapToKeys(const toml::table& keys_table, Keys& keys);
};
