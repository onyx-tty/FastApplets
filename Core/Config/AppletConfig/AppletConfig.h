// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"

#include <toml++/toml.hpp>

namespace config {

class Filepaths;

struct AppletConfig final {
        schema::Config config;
        schema::Config default_config;
        schema::Keys   keys;
        schema::Keys   default_keys;
};

template<applet::type TApplet>
requires(TApplet != applet::type::global)
[[nodiscard]] AppletConfig makeAppletConfig(const Filepaths& applet, const Filepaths& global);

template<applet::type TApplet>
requires(TApplet != applet::type::global)
[[nodiscard]] auto makeAppletConfigTuple(const Filepaths& applet, const Filepaths& global);

} // namespace config

#include "AppletConfig.tpp"
