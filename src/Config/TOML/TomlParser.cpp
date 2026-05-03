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

#include "TomlParser.h"
#include "CppUtils/include/CppUtils/Log/QtLog.h"
#include "TomlLocator.h"
#include "Types/ConfigTomlFiles.h"

#include <string>
#include <toml++/toml.hpp>

toml::table TomlParser::createTable(const std::string& file_path) {
        toml::table file_table;

        try {
                file_table = toml::parse_file(file_path);
                QDEBUG() << "Parsed file" << QString::fromStdString(file_path);
        } catch (const toml::parse_error& error) {
                QFATAL("Parsing of %s failed: %s", std::string(file_path).c_str(),
                       std::string(error.description()).c_str());
        }

        return file_table;
}

// TODO Shorter names
ConfigTomlFiles TomlParser::global_toml_files       = TomlLocator::locateGlobalConfigFiles();
ConfigTomlFiles TomlParser::power_applet_toml_files = TomlLocator::locatePowerAppletConfigFiles();

// TODO Accept applet enum as parameter, return the right config with just 1 function
const toml::table& TomlParser::parseGlobalConfig() {
        static toml::table config = createTable(global_toml_files.config);
        return config;
}

const toml::table& TomlParser::parseGlobalKeys() {
        static toml::table keys = createTable(global_toml_files.keys);
        return keys;
}

const toml::table& TomlParser::parsePowerAppletConfig() {
        static toml::table config = createTable(power_applet_toml_files.config);
        return config;
}

const toml::table& TomlParser::parsePowerAppletKeys() {
        static toml::table keys = createTable(power_applet_toml_files.keys);
        return keys;
}
