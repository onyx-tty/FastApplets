// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigManager.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Core/Config/Factory/ConfigFactory.h"
#include "Core/Config/FileLocator/FileLocator.h"
#include "Core/Config/KeysFile/Mapper/KeysMapper.h"
#include "Core/Config/TomlParser/TomlParser.h"
#include "Core/Config/Types/ConfigFilepaths.h"
#include "Core/Config/Types/ConfigType.h"

template<applet::type TApplet>
const ConfigFilepaths& configFilepaths() {
        static ConfigFilepaths filepaths = FileLocator::configFiles(applet::toLatin1String(TApplet));
        return filepaths;
}

// TODO: Collapse the if chain by creating ConfigTraits
// TODO: Return type deduction prevents optimizations
template<applet::type TApplet>
template<config::type TConfigFile>
const auto& ConfigManager<TApplet>::get() {
        static_assert(
                TApplet != applet::type::global,
                "Passing applet::type::global is an error! It will result in duplicate global nodes!");

        const auto& applet_files = configFilepaths<TApplet>();
        const auto& global_files = configFilepaths<applet::type::global>();

        if constexpr (TConfigFile == config::type::config) {
                static const TConfig config =
                        ConfigMapper::config<TApplet>(TomlParser::file(applet_files.config),
                                                      TomlParser::file(global_files.config),
                                                      getDefault<TConfigFile>());
                return config;
        } else {
                static TKeys keys = KeysMapper::keys<TApplet>(TomlParser::file(applet_files.keys),
                                                              TomlParser::file(global_files.keys),
                                                              getDefault<TConfigFile>());
                return keys;
        }
}

// TODO: Collapse the if chain by creating ConfigTraits
// TODO: Return type deduction prevents optimizations
template<applet::type TApplet>
template<config::type TConfigFile>
const auto& ConfigManager<TApplet>::getDefault() {
        if constexpr (TConfigFile == config::type::config) {
                static const TConfig default_config = ConfigFactory<TApplet>::createDefaultConfig();
                return default_config;
        } else {
                static const TKeys default_keys = ConfigFactory<TApplet>::createDefaultKeys();
                return default_keys;
        }
}
