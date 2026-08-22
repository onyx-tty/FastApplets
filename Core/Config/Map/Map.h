// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"

#include <toml++/toml.hpp>

namespace config { class ConfigView; } // namespace config

namespace config::map::helpers {}

// Maps TOML configuration to the schemas.
//
// All mapping failures will fall back to hardcoded defaults and log warnings.
// Malformed values (wrong type, out of range, etc.) are treated as failures.
namespace config::map {

using namespace config::map::helpers;
using config::ConfigView;
using config::schema::Config;
using config::schema::Keys;

// Parses applet and global tables into config::schema::Config.
//
// Usage:
//   auto config = config::map::config<TApplet>(applet, global, defaults);
//
// The applet table supplies primary configuration and overrides, global
// provides fallbacks.
//
// QApplication must exist before calling.
template<applet::Type TApplet>
[[nodiscard]] Config config(
        const toml::table& applet, const toml::table& global, const Config& defaults);

// Parses applet and global tables into config::schema::Keys.
//
// Usage:
//   auto keys = config::map::keys<TApplet>(applet, global, defaults);
//
// The applet table supplies primary configuration and overrides, global
// provides fallbacks.
//
// QApplication must exist before calling.
template<applet::Type TApplet>
[[nodiscard]] Keys keys(const toml::table& applet, const toml::table& global, const Keys& defaults);

} // namespace config::map

#include "Map.tpp"
