// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FileLocator.h"
#include "Config/Types/ConfigTomlFiles.h"
#include "CppUtils/Log/QtLog.h"

#include <string>
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <QStringLiteral>
#include <QStringView>

std::string findFile(QStringView filename, QStringView subdirectory) {
        QString subdir   = subdirectory.empty() ? subdirectory.toString()
                                                : subdirectory.toString() + "/";
        QString filepath = qEnvironmentVariable("XDG_CONFIG_HOME") + "/FastApplets/" + subdir
                         + filename.toString();

        // If file found, save filepath
        if (QFileInfo::exists(filepath)) { return filepath.toStdString(); }

        QFATAL("%s not found!", filepath.toStdString().c_str());
}

ConfigTomlFiles FileLocator::locateConfigFiles(std::string_view scope) {
        ConfigTomlFiles files{};

        // Treat scope "global" as root
        if (scope == "global") {
                scope = "";
        }

        files.config = findFile(QStringLiteral("config.toml"),
                                QString::fromStdString(std::string{scope}));
        files.keys   = findFile(QStringLiteral("keys.toml"),
                                QString::fromStdString(std::string{scope}));

        return files;
}
