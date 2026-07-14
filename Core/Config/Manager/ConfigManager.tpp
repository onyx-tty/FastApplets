// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigManager.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Core/Config/Defaults/Defaults.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Mapper/KeysMapper.h"
#include "Core/Config/TomlParser/TomlParser.h"
#include "Core/Config/Types/ConfigFilepaths.h"
#include "Core/Config/Types/ConfigType.h"

#include <tuple>
#include <QDebug>
#include <QtGlobal>

template<applet::type TApplet>
config::Manager<TApplet>::Data::Data() :
        config(Config{}), default_config(Config{}), keys(Keys{}), default_keys(Keys{}),
        is_setup(false) {}

template<applet::type TApplet>
config::Manager<TApplet>::Data& config::Manager<TApplet>::getData() {
        static Data instance = {};
        return instance;
}

template<applet::type TApplet>
void config::Manager<TApplet>::setup(const Filepaths& applet_filepaths,
                                     const Filepaths& global_filepaths) {
        auto& data = getData();

        data.default_config = makeDefaultConfig<TApplet>();
        data.config         = ConfigMapper::config<TApplet>(parseTomlFile(applet_filepaths.config),
                                                            parseTomlFile(global_filepaths.config),
                                                            data.default_config);

        data.default_keys = makeDefaultKeys<TApplet>();
        data.keys         = KeysMapper::keys<TApplet>(parseTomlFile(applet_filepaths.keys),
                                                      parseTomlFile(global_filepaths.keys),
                                                      data.default_keys);

        data.is_setup = true;
}

// TODO: Collapse the if chain by creating ConfigTraits
// TODO: Return type deduction prevents optimizations
template<applet::type TApplet>
template<config::type TConfigFile>
const auto& config::Manager<TApplet>::get(Defaults defaults) {
        static_assert(
                TApplet != applet::type::global,
                "Passing applet::type::global is an error! It will result in duplicate global nodes!");

        auto& data = getData();

        if (!data.is_setup) { qFatal("config::Manager is not set up yet"); }

        if constexpr (TConfigFile == type::config) {
                if (defaults.defaults) { return data.default_config; }

                return data.config;
        } else {
                if (defaults.defaults) { return data.default_keys; }

                return data.keys;
        }
}

template<applet::type TApplet>
auto config::Manager<TApplet>::getAll() {
        return std::forward_as_tuple(get<type::config>({.defaults = false}),
                                     get<type::keys>({.defaults = false}),
                                     get<type::keys>({.defaults = true}));
}
