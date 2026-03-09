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

#include <toml++/toml.hpp>

class Keys;

class KeysMapper final {
private:
        KeysMapper() = delete;

        /* Global Keys */
        static void mapGlobalQuitKeys(const toml::table& global, Keys& keys);
        static void mapGlobalKeys(const toml::table& keys_table, Keys& keys);

        /* Power Applet Keys*/
        static void mapPowerAppletQuitKeys(const toml::table& power_applet, Keys& keys);
        static void mapPowerAppletPrimaryButtonKeys(const toml::table& power_applet, Keys& keys);
        static void mapPowerAppletKeys(const toml::table& keys_table, Keys& keys);

public:
        static void mapToKeys(const toml::table& keys_table, Keys& keys);
};
