// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Schema/Config.h"
#include "Core/Config/KeysFile/Schema/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>
#include <Qt>
#include <QtGlobal>

// TODO: Move elsewhere
[[nodiscard]] inline std::vector<keybindings> makeKeyRange(Qt::Key first, Qt::Key last) {
        if (first > last) { qFatal("First must come before last"); }

        std::vector<keybindings> ret = {};
        ret.reserve(last - first + 1);

        for (int key = first; key <= last; ++key) { ret.push_back(keybindings{key}); }

        return ret;
}

namespace config {

using Config = schema::Config;
using Keys   = schema::Keys;

// Construct default Config for applet TApplet.
template<applet::type TApplet>
[[nodiscard]] Config makeDefaultConfig();

// Construct default config::schema::Keys for applet TApplet.
template<applet::type TApplet>
[[nodiscard]] Keys makeDefaultKeys();

} // namespace config

#include "Defaults.tpp"
