// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
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
public:
        ConfigManager() = delete;

        template<config::type TConfigFile>
        static const auto& get();

        template<config::type TConfigFile>
        static const auto& getDefault();
};

#include "ConfigManager.tpp"
