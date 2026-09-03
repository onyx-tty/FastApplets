// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Keybindings.h"

#include <toml++/toml.hpp>
#include <tuple>

namespace config {

class Filepaths;

using applet_config = std::tuple<schema::Config, schema::Keys>;

// Returns complete, mapped configs required by an applet.
template<applet::Type TApplet>
requires(TApplet != applet::Type::Global)
[[nodiscard]] applet_config makeAppletConfig(
        const Filepaths& applet, const Filepaths& global, keybindings& claimed_keys);

} // namespace config

#include "AppletConfig.tpp"
