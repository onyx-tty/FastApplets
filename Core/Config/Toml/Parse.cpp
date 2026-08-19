// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Parse.h"

#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QtGlobal>
#include <string>
#include <toml++/toml.hpp>

toml::table config::parseTomlFile(const QString& filepath) {
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
