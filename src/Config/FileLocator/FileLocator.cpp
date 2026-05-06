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

static std::string findFile(QStringView filename, QStringView subdirectory = QStringLiteral("")) {
        QString subdir   = subdirectory.empty() ? subdirectory.toString()
                                                : subdirectory.toString() + "/";
        QString filepath = qEnvironmentVariable("XDG_CONFIG_HOME") + "/FastApplets/" + subdir
                         + filename.toString();

        // If file found, save filepath
        if (QFileInfo::exists(filepath)) { return filepath.toStdString(); }

        QFATAL("%s not found!", filepath.toStdString().c_str());
}

// Look for configs in $XDG_CONFIG_HOME/FastApplets
ConfigTomlFiles FileLocator::locateGlobalConfigFiles() {
        ConfigTomlFiles files{};

        files.config = findFile(QStringLiteral("config.toml"));
        files.keys   = findFile(QStringLiteral("keys.toml"));

        return files;
}

ConfigTomlFiles FileLocator::locatePowerAppletConfigFiles() {
        ConfigTomlFiles files{};

        files.config = findFile(QStringLiteral("config.toml"), QStringLiteral("power_applet"));
        files.keys   = findFile(QStringLiteral("keys.toml"), QStringLiteral("power_applet"));

        return files;
}
