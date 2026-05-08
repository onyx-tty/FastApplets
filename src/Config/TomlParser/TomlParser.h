// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletTypes.h"
#include "Config/Types/ConfigType.h"

#include <string>
#include <toml++/toml.hpp>

class ConfigTomlFiles;

class TomlParser final {
private:
        // TODO Shorter names
        static ConfigTomlFiles global_toml_files;
        static ConfigTomlFiles power_applet_toml_files;

        static toml::table createTable(const std::string& file_path);

public:
        TomlParser() = delete;
        static toml::table parseGlobalConfig();
        static toml::table parseGlobalKeys();
        static toml::table parsePowerAppletConfig();
        static toml::table parsePowerAppletKeys();
};
