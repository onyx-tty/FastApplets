// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigManager.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Core/Config/Factory/ConfigFactory.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Mapper/KeysMapper.h"
#include "Core/Config/TomlParser/TomlParser.h"
#include "Core/Config/Types/ConfigFilepaths.h"
#include "Core/Config/Types/ConfigType.h"

#include <QDebug>
#include <QtGlobal>

template<applet::type TApplet>
ConfigManager<TApplet>::Data::Data() :
        config(Config{}), default_config(Config{}), keys(Keys{}), default_keys(Keys{}),
        is_setup(false) {}

template<applet::type TApplet>
ConfigManager<TApplet>::Data& ConfigManager<TApplet>::getData() {
        static Data instance = {};
        return instance;
}

template<applet::type TApplet>
void ConfigManager<TApplet>::setup(const ConfigFilepaths& applet_filepaths,
                                   const ConfigFilepaths& global_filepaths) {
        auto& data = getData();

        data.default_config = ConfigFactory<TApplet>::createDefaultConfig();
        data.config = ConfigMapper::config<TApplet>(TomlParser::file(applet_filepaths.config),
                                                    TomlParser::file(global_filepaths.config),
                                                    data.default_config);

        data.default_keys = ConfigFactory<TApplet>::createDefaultKeys();
        data.keys         = KeysMapper::keys<TApplet>(TomlParser::file(applet_filepaths.keys),
                                                      TomlParser::file(global_filepaths.keys),
                                                      data.default_keys);

        data.is_setup = true;
}

// TODO: Collapse the if chain by creating ConfigTraits
// TODO: Return type deduction prevents optimizations
template<applet::type TApplet>
template<config::type TConfigFile>
const auto& ConfigManager<TApplet>::get(Defaults defaults) {
        static_assert(
                TApplet != applet::type::global,
                "Passing applet::type::global is an error! It will result in duplicate global nodes!");

        auto& data = getData();

        if (!data.is_setup) { qFatal("ConfigManager is not set up yet"); }

        if constexpr (TConfigFile == config::type::config) {
                if (defaults.defaults) { return data.default_config; }

                return data.config;
        } else {
                if (defaults.defaults) { return data.default_keys; }

                return data.keys;
        }
}
