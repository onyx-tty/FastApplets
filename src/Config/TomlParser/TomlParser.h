// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <toml++/toml.hpp>

class ConfigTomlFiles;

class TomlParser final {
private:
        static ConfigTomlFiles global_toml_files;
        static ConfigTomlFiles power_applet_toml_files;

        static toml::table createTable(const std::string& file_path);

public:
        TomlParser() = delete;
        static const toml::table& parseGlobalConfig();
        static const toml::table& parseGlobalKeys();
        static const toml::table& parsePowerAppletConfig();
        static const toml::table& parsePowerAppletKeys();
};
