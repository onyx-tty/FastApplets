// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <toml++/toml.hpp>
#include <vector>
#include <Qt>
#include <QtGlobal>

class ConfigFilepaths;

inline std::vector<keybindings> makeKeyRange(Qt::Key first, Qt::Key last) {
        if (first > last) { qFatal("First must come before last"); }

        std::vector<keybindings> ret = {};
        ret.reserve(last - first + 1);

        for (int key = first; key <= last; ++key) { ret.push_back(keybindings{key}); }

        return ret;
}

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

        static const ConfigFilepaths& configFilepaths(applet::type applet);
        static TConfig                makeDefaultConfig();
        static TKeys                  makeDefaultKeys();

public:
        ConfigManager() = delete;
        static const TConfig& getConfig();
        static const TConfig& getDefaultConfig();
        static const TKeys&   getKeys();
        static const TKeys&   getDefaultKeys();
};

#include "ConfigManager.tpp"
