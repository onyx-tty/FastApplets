/* FastApplets

   Copyright (C) 2025 Łukasz Wrodarczyk

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

class TomlConfigParser final {
private:
        const toml::table config_table;
        const toml::table keys_table;

        TomlConfigParser();

        void parseWindowProperties();
        void parseButtonProperties();
        void parseLayoutProperties();
        void parseKeys();

public:
        TomlConfigParser(const TomlConfigParser&) = delete;
        TomlConfigParser(TomlConfigParser&&) = delete;
        TomlConfigParser& operator=(const TomlConfigParser&) = delete;
        TomlConfigParser& operator=(TomlConfigParser&&) = delete;

        static TomlConfigParser& getInstance();
        void parseConfig();
};
