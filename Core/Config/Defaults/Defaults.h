// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Keybindings.h"

#include <Qt>
#include <QtGlobal>

namespace config {

using schema::Config;
using schema::Keys;

// Construct default config::schema::Config for applet TApplet.
template<applet::Type TApplet>
[[nodiscard]] Config makeDefaultConfig();

// Construct default config::schema::Keys for applet TApplet.
template<applet::Type TApplet>
[[nodiscard]] Keys makeDefaultKeys(keybindings& claimed_keys);

} // namespace config

#include "Defaults.tpp"
