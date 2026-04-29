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

#include "TomlLocator.h"
#include "CppUtils/include/Log/QtLog.h"
#include "Types/ConfigTomlFiles.h"

#include <string>
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <QStringView>

static std::string findFile(const QStringView& filename,
                            const QStringView& subdirectory = QStringLiteral("")) {
        QString subdir   = subdirectory.empty() ? subdirectory.toString()
                                                : subdirectory.toString() + "/";
        QString filepath = qEnvironmentVariable("XDG_CONFIG_HOME") + "/FastApplets/" + subdir
                         + filename.toString();

        // If file found, save filepath
        if (QFileInfo::exists(filepath)) { return filepath.toStdString(); }

        QFATAL("%s not found!", filepath.toStdString().c_str());
}

// Look for configs in $XDG_CONFIG_HOME/FastApplets
ConfigTomlFiles TomlLocator::locateGlobalConfigFiles() {
        ConfigTomlFiles files{};

        files.config = findFile(QStringLiteral("config.toml"));
        files.keys   = findFile(QStringLiteral("keys.toml"));

        return files;
}

ConfigTomlFiles TomlLocator::locatePowerAppletConfigFiles() {
        ConfigTomlFiles files{};

        files.config = findFile(QStringLiteral("config.toml"), QStringLiteral("power_applet"));
        files.keys   = findFile(QStringLiteral("keys.toml"), QStringLiteral("power_applet"));

        return files;
}
