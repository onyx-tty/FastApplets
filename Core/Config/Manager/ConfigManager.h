// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/Types/ConfigType.h"

#include <toml++/toml.hpp>

namespace config {

class Filepaths;

// Stores instances of Config and Keys schemas for each applet called.
// Initializes instances from ConfigMapper and KeysMapper.
template<applet::type TApplet>
class Manager final {
private:
        struct Data {
                config::schema::Config config;
                config::schema::Config default_config;
                config::schema::Keys   keys;
                config::schema::Keys   default_keys;
                bool                   is_setup;

                Data();
        };

        static Data& getData();

public:
        Manager() = delete;

        struct Defaults final {
                bool defaults = false;
        };

        static void setup(const config::Filepaths& applet_filepaths,
                          const config::Filepaths& global_filepaths);

        template<config::type TConfigFile>
        static const auto& get(Defaults defaults);
        static auto        getAll();
};

} // namespace config

#include "ConfigManager.tpp"
