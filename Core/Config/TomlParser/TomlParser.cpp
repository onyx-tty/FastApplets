// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TomlParser.h"

#include <string>
#include <toml++/toml.hpp>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QtGlobal>

toml::table TomlParser::file(const QString& filepath) {
        toml::table file = {};

        if (!QFileInfo::exists(filepath)) {
                qWarning() << "Nothing to parse";
                return {};
        }

        try {
                file = toml::parse_file(filepath.toStdString());
                qDebug() << "Parsed file" << filepath;
        } catch (const toml::parse_error& error) {
                qWarning() << QString("%1: %2").arg(filepath).arg(
                        QString::fromStdString(std::string(error.description())));
                return {};
        }

        return file;
}
