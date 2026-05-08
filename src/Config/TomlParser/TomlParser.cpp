// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TomlParser.h"
#include "Applets/Types/AppletRecord.h"
#include "Applets/Types/AppletTypes.h"
#include "Config/FileLocator/FileLocator.h"
#include "Config/Types/ConfigTomlFiles.h"
#include "Config/Types/ConfigType.h"
#include "CppUtils/Log/QtLog.h"

#include <string>
#include <toml++/toml.hpp>

ConfigTomlFiles TomlParser::global_toml_files = FileLocator::locateConfigFiles(applet::global.scope);
ConfigTomlFiles TomlParser::power_applet_toml_files = FileLocator::locateConfigFiles(
        applet::power_applet.scope);

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

toml::table TomlParser::parseGlobalConfig() {
        return createTable(global_toml_files.config);
}

toml::table TomlParser::parseGlobalKeys() {
        return createTable(global_toml_files.keys);
}

toml::table TomlParser::parsePowerAppletConfig() {
        return createTable(power_applet_toml_files.config);
}

toml::table TomlParser::parsePowerAppletKeys() {
        return createTable(power_applet_toml_files.keys);
}
