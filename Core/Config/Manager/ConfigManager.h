// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/Types/ConfigType.h"

#include <toml++/toml.hpp>

class ConfigFilepaths;

// Returns cached locations of each config.
template<applet::type TApplet>
static const ConfigFilepaths& configFilepaths();

// Stores instances of Config and Keys schemas for each applet called.
// Initializes instances from ConfigMapper and KeysMapper.
template<applet::type TApplet>
class ConfigManager final {
private:
        struct Data {
                Config config;
                Config default_config;
                Keys   keys;
                Keys   default_keys;
                bool   is_setup;

                Data();
        };

        static void  setup();
        static Data& getData();

public:
        ConfigManager() = delete;

        struct Defaults final {
                bool defaults = false;
        };

        template<config::type TConfigFile>
        static const auto& get(Defaults defaults);
};

#include "ConfigManager.tpp"
