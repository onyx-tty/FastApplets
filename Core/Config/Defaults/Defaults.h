// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Keybindings.h"

#include <Qt>
#include <QtGlobal>
#include <vector>

// TODO: Move elsewhere
[[nodiscard]] inline std::vector<keybindings> makeKeyRange(Qt::Key first, Qt::Key last) {
        if (first > last) { qFatal("First must come before last"); }

        std::vector<keybindings> ret = {};
        ret.reserve(last - first + 1);

        for (int key = first; key <= last; ++key) { ret.push_back(keybindings{key}); }

        return ret;
}

namespace config {

using schema::Config;
using schema::Keys;

// Construct default Config for applet TApplet.
template<applet::Type TApplet>
[[nodiscard]] Config makeDefaultConfig();

// Construct default config::schema::Keys for applet TApplet.
template<applet::Type TApplet>
[[nodiscard]] Keys makeDefaultKeys();

} // namespace config

#include "Defaults.tpp"
