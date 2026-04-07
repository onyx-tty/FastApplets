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
#include "Core/Log.h"
#include "Types/ConfigTomlFiles.h"

#include <string>
#include <QFileInfo>
#include <QString>
#include <QStringView>
#include <QtEnvironmentVariables>

static std::string findFile(const QStringView& filename) {
        QString filepath = qEnvironmentVariable("XDG_CONFIG_HOME") + "/FastApplets/" + filename;

        // If file found, save filepath
        if (QFileInfo::exists(filepath)) { return filepath.toStdString(); }

        QFATAL("%s not found!", filepath);
}

// Look for configs in $XDG_CONFIG_HOME
ConfigTomlFiles TomlLocator::locateTomlFiles() {
        ConfigTomlFiles files{};

        files.config = findFile(QStringLiteral("config.toml"));
        files.keys   = findFile(QStringLiteral("keys.toml"));

        return files;
}
