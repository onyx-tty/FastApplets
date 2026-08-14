// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"

#include <toml++/toml.hpp>
#include <tuple>

namespace config {

class Filepaths;

using applet_config = std::tuple<schema::Config, schema::Config, schema::Keys, schema::Keys>;

template<applet::Type TApplet>
requires(TApplet != applet::Type::Global)
[[nodiscard]] applet_config makeAppletConfig(const Filepaths& applet, const Filepaths& global);

} // namespace config

#include "AppletConfig.tpp"
