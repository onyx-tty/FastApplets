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

#include <array>
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
std::array<std::string, toml_file_names_cnt> TomlLocator::locateTomlFiles() {
        std::array<std::string, toml_file_names_cnt> files{}; // Only enough slots for each file
        std::array<QString, toml_file_names_cnt> config_file_names = {"config.toml", "keys.toml"};

        QString file_path;
        // Loop through expected config files
        // If no valid file_path found for the current file, terminate
        // TODO Fallback into default config
        for (size_t i = 0; i != toml_file_names_cnt; ++i) {
                files[i] = findFile(config_file_names[i]);
        }

        return files;
}
