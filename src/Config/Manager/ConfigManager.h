// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletTraits.h"
#include "Applets/Types/AppletType.h"

#include <toml++/toml.hpp>

class ConfigFiles;

// Stores instances of Config and Keys schemas.
// Initializes instances from ConfigMapper and KeysMapper.
template<applet::type TApplet>
class ConfigManager final {
private:
        using TConfig              = AppletTraits<TApplet>::TConfig;
        using TKeys                = AppletTraits<TApplet>::TKeys;
        using TPrimaryButton       = AppletTraits<TApplet>::TPrimaryButton;
        using TPrimaryButtonParams = AppletTraits<TApplet>::TPrimaryButtonParams;
        using TPrimaryButtonType   = AppletTraits<TApplet>::TPrimaryButtonType;

        static const ConfigFiles& configFilepaths(applet::type applet);
        static TConfig            makeDefaultConfig();
        static TKeys              makeDefaultKeys();

public:
        ConfigManager() = delete;
        static const TConfig& getConfig();
        static const TConfig& getDefaultConfig();
        static const TKeys&   getKeys();
        static const TKeys&   getDefaultKeys();
};

#include "ConfigManager.tpp"
