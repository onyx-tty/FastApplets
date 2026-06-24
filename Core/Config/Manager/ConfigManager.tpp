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

#include <cassert>
#include <utility>

// TODO: TApplet is unused
// TODO: Remove switch, rely on template, it will generate a static for each template overload
template<applet::type TApplet>
const ConfigFilepaths& ConfigManager<TApplet>::configFilepaths(applet::type applet) {
        switch (applet) {
        case applet::type::power_applet:
                static const ConfigFilepaths power = FileLocator::configFiles(
                        applet::toLatin1String(applet));
                return power;
        case applet::type::action_applet:
                static const ConfigFilepaths action = FileLocator::configFiles(
                        applet::toLatin1String(applet));
        case applet::type::global:
                static const ConfigFilepaths global = FileLocator::configFiles(
                        applet::toLatin1String(applet));
                return global;
        // TODO: Use qFatal instead
        default: assert(false && "Passed unknown type"); std::unreachable();
        }
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TConfig& ConfigManager<TApplet>::getConfig() {
        const auto& applet_files = configFilepaths(TApplet);
        const auto& global_files = configFilepaths(applet::type::global);

        static const TConfig config =
                ConfigMapper::config<TApplet>(TomlParser::file(applet_files.config),
                                              TomlParser::file(global_files.config),
                                              getDefaultConfig());

        return config;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TConfig& ConfigManager<TApplet>::getDefaultConfig() {
        static const TConfig default_config = ConfigFactory<TApplet>::createDefaultConfig();

        return default_config;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TKeys& ConfigManager<TApplet>::getKeys() {
        const auto& applet_files = configFilepaths(TApplet);
        const auto& global_files = configFilepaths(applet::type::global);

        static TKeys keys = KeysMapper::keys<TApplet>(TomlParser::file(applet_files.keys),
                                                      TomlParser::file(global_files.keys),
                                                      getDefaultKeys());

        return keys;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TKeys& ConfigManager<TApplet>::getDefaultKeys() {
        static const TKeys default_keys = ConfigFactory<TApplet>::createDefaultKeys();

        return default_keys;
}
