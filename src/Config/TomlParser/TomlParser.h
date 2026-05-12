// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletTypes.h"
#include "Config/Types/ConfigType.h"

#include <string>
#include <toml++/toml.hpp>

class ConfigFiles;

// Parses TOML configuration files for different applets and config files
//
// This is a low-level file parser used to obtain toml::tables for mapping.
//
// File locations are determined once at startup via FileLocator and cached
// in static members.
// TODO inline cached TOML files for safety
// Parse errors are fatal and immediately terminate the application.
// TODO Handle parse errors, in which case pass empty toml::table without any problems.
//
// Available combinations:
//   - global + config       -> global config.toml
//   - global + keys         -> global keys.toml
//   - power_applet + config -> power_applet config.toml
//   - power_applet + keys   -> power_applet keys.toml
class TomlParser final {
private:
        // Cached file paths for each applet scope
        // TODO Shorter names
        static ConfigFiles global_toml_files;
        static ConfigFiles power_applet_toml_files;

        // Parses a single file or fatals on error
        static toml::table createTable(const std::string& file_path);

public:
        TomlParser() = delete;

        // Parses the requested config file into a TOML table, corresponding
        // to the type of config and applet.
        //
        // Returns: toml::table representing the parsed file.
        //          Never returns empty table. Parse errors are fatal.
        //
        // Note: Parse errors crash the application with QFATAL.
        static toml::table parseFile(applet::type applet, config::type config);
};
