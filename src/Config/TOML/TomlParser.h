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

#include "TomlLocator.h"

#include <qnamespace.h>
#include <string>
#include <toml++/toml.hpp>
#include <QKeyCombination>
#include <QKeySequence>
#include <QSizePolicy>
#include <QString>

class TomlParser final {
private:
        static std::array<std::string, toml_file_names_cnt> toml_files;

        static toml::table createTable(std::string file_path);

public:
        TomlParser() = delete;
        static const toml::table& parseConfig();
        static const toml::table& parseKeys();
};
