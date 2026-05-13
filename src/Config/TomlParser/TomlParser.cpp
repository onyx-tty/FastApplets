// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TomlParser.h"
#include "Applets/Types/AppletRecord.h"
#include "Applets/Types/AppletType.h"
#include "Config/FileLocator/FileLocator.h"
#include "Config/Types/ConfigFiles.h"
#include "Config/Types/ConfigType.h"
#include "CppUtils/Log/QtLog.h"

#include <string>
#include <toml++/toml.hpp>
#include <QFileInfo>

ConfigFiles TomlParser::global_toml_files = FileLocator::locateConfigFiles(applet::global.scope);
ConfigFiles TomlParser::power_applet_toml_files = FileLocator::locateConfigFiles(
        applet::power_applet.scope);

toml::table TomlParser::createTable(const std::string& file_path) {
        toml::table file_table{};

        if (!QFileInfo::exists(QString::fromStdString(file_path))) {
                QWARNING() << "File not found!";
                return {};
        }

        try {
                file_table = toml::parse_file(file_path);
                QDEBUG() << "Parsed file" << QString::fromStdString(file_path);
        } catch (const toml::parse_error& error) {
                QWARNING() << QString("%1:").arg(QString::fromStdString(file_path))
                           << QString::fromStdString(std::string{error.description()});
                return {};
        }

        return file_table;
}

toml::table TomlParser::parseFile(applet::type applet, config::type config) {
        ConfigFiles* files = nullptr;
        switch (applet) {
        case applet::type::power_applet: files = &power_applet_toml_files; break;
        case applet::type::global:       files = &global_toml_files; break;
        default:
                QWARNING() << "Unsupported applet::type found:" << static_cast<int>(applet);
                return {};
        }

        std::string* file = nullptr;
        switch (config) {
        case config::type::config: file = &files->config; break;
        case config::type::keys:   file = &files->keys; break;
        default:
                QWARNING() << "Unsupported config::type found:", static_cast<int>(config);
                return {};
        }

        return createTable(*file);
}
