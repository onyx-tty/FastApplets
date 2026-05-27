// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TomlParser.h"
#include "CppUtils/Log/QtLog.h"

#include <string>
#include <string_view>
#include <toml++/toml.hpp>
#include <QFileInfo>
#include <QString>

toml::table TomlParser::file(std::string_view filepath) {
        toml::table file = {};

        if (!QFileInfo::exists(QString::fromStdString(std::string{filepath}))) {
                QWARNING() << "File not found!";
                return {};
        }

        try {
                file = toml::parse_file(filepath);
                QDEBUG() << "Parsed file" << QString::fromStdString(std::string{filepath});
        } catch (const toml::parse_error& error) {
                QWARNING() << QString("%1:").arg(QString::fromStdString(std::string{filepath}))
                           << QString::fromStdString(std::string{error.description()});
                return {};
        }

        return file;
}
