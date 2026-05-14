// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TomlParser.h"
#include "CppUtils/Log/QtLog.h"

#include <string>
#include <toml++/toml.hpp>
#include <QFileInfo>
#include <QString>

toml::table TomlParser::parseFile(const std::string& filepath) {
        toml::table file_table{};

        if (!QFileInfo::exists(QString::fromStdString(filepath))) {
                QWARNING() << "File not found!";
                return {};
        }

        try {
                file_table = toml::parse_file(filepath);
                QDEBUG() << "Parsed file" << QString::fromStdString(filepath);
        } catch (const toml::parse_error& error) {
                QWARNING() << QString("%1:").arg(QString::fromStdString(filepath))
                           << QString::fromStdString(std::string{error.description()});
                return {};
        }

        return file_table;
}
