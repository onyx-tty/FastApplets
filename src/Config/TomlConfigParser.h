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

const toml::table& createConfig();
const toml::table& createKeys();

class TomlConfigParser final {
private:
        TomlConfigParser() = delete;

        static void parseWindowProperties(const toml::table& config_table);
        static void parseButtonProperties(const toml::table& config_table);
        static void parseLayoutProperties(const toml::table& config_table);

public:
        static void parseConfig(const toml::table& config_table);
        static void parseKeys(const toml::table& keys_table);
};
