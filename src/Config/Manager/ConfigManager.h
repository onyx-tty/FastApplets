// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// TODO: Use templates instead
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"

// Stores instances of Config and Keys schemas.
// Initializes instances from ConfigMapper and KeysMapper.
class ConfigManager final {
private:
        static PowerAppletConfig makeDefaultConfig();
        static PowerAppletKeys   makeDefaultKeys();

        // TODO: Handle cert-err58-cpp
        inline static PowerAppletConfig config         = PowerAppletConfig{};
        inline static PowerAppletConfig default_config = PowerAppletConfig{};
        inline static PowerAppletKeys   keys           = PowerAppletKeys{};
        inline static PowerAppletKeys   default_keys   = PowerAppletKeys{};
        inline static bool              initialized    = false;

public:
        ConfigManager() = delete;
        // Finds all TOML config filepaths, parses them, and maps to instances.
        static void                     init();
        static const PowerAppletConfig& getConfig();
        static const PowerAppletConfig& getDefaultConfig();
        static const PowerAppletKeys&   getKeys();
        static const PowerAppletKeys&   getDefaultKeys();
};
