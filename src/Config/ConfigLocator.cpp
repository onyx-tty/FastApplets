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

#include "ConfigLocator.h"
#include "Core/Log.h"

#include <array>
#include <string>
#include <QFileInfo>
#include <QString>
#include <QtEnvironmentVariables>

// Values needed to find configs
namespace {

static std::array<QString, 2> config_dir_paths = {qEnvironmentVariable("XDG_CONFIG_HOME")
                                                          + "/FastApplets/",
                                                  qEnvironmentVariable("XDG_DATA_HOME")
                                                          + "/FastApplets/"};
static std::array<QString, config_file_names_cnt> config_file_names = {"config.toml", "keys.toml"};

} // namespace

// TODO Split
// Look for configs in $XDG_CONFIG_HOME and $XDG_DATA_HOME
std::array<std::string, config_file_names_cnt> ConfigLocator::locateConfigFiles() {
        std::array<std::string, config_file_names_cnt> files{}; // Only enough slots for each file

        QString file_path;
        // Loop through expected config files
        for (size_t file_i = 0; file_i != config_file_names_cnt; ++file_i) {
                bool found = false;
                // Loop through expected directories
                for (size_t dir_i = 0; !found && dir_i != config_dir_paths_cnt; ++dir_i) {
                        file_path = config_dir_paths[dir_i] + config_file_names[file_i];
                        // If file found, save filepath, stop the loop for that file
                        if (QFileInfo::exists(file_path)) {
                                files[file_i] = file_path.toStdString();
                                found         = true;
                                break;
                        }
                }

                // No valid file_path found for the current file, terminate
                // TODO Generate default TOML file on failure
                if (!found) {
                        QFATAL("%s not found!", config_file_names[file_i].toStdString().c_str());
                }
        }

        return files;
}
