// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>
#include <Qt>
#include <QtGlobal>

// TODO: Move elsewhere
inline std::vector<keybindings> makeKeyRange(Qt::Key first, Qt::Key last) {
        if (first > last) { qFatal("First must come before last"); }

        std::vector<keybindings> ret = {};
        ret.reserve(last - first + 1);

        for (int key = first; key <= last; ++key) { ret.push_back(keybindings{key}); }

        return ret;
}

// Returns constructed default config and keys associated with given applet.
template<applet::type TApplet>
class ConfigFactory final {
public:
        ConfigFactory() = delete;
        static Config createDefaultConfig();
        static Keys   createDefaultKeys();
};

#include "ConfigFactory.tpp"
