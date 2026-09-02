// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/View/View.h"
#include "Core/Shell/Types/ShellContext.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <TomlQt/ArrayBounds.h>
#include <optional>
#include <toml++/toml.hpp>
#include <vector>

namespace config::map::helpers {

using config::ConfigView;
using tomlqt::ArrayBounds;

/* ShellContext */

// Expected format: environment table containing terminate_on_command_exit (bool)
[[nodiscard]] ShellContext shellContext(const ConfigView& node, const ShellContext& defaults);

/* WindowParams */

// Expected format: window table containing title (string) and
//                  size (array of two integers)
[[nodiscard]] WindowParams windowParams(const ConfigView& node, const WindowParams& defaults);

/* PrimaryButtonParams */

// Expected format: primary_button table containing double_key_press (bool),
//                  text_alignment (string), icon_alignment (string),
//                  icon_size (array of two integers), policy (string), and
//                  list (array of tables)
template<applet::Type TApplet>
[[nodiscard]] PrimaryButtonParams primaryButtonParams(
        const ConfigView& node, const PrimaryButtonParams& defaults);

// Expected format: array of tables
template<applet::Type TApplet>
[[nodiscard]] std::vector<PerPrimaryButtonParams> perPrimaryButtonParamsList(
        const ConfigView& node, const std::vector<PerPrimaryButtonParams>& defaults);

// Expected format: primary_buttons.list[index] table containing type (string),
//                  text (string), and command (string)
//
// Regarding PerPrimaryButtonParams::command: QProcess::splitCommand() does not
// interpret single quotes as quotes, but as regular characters. This function
// converts single quotes to double quotes to work around that limitation.
template<applet::Type TApplet>
[[nodiscard]] std::optional<PerPrimaryButtonParams> perPrimaryButtonParams(const ConfigView& node);

// Expected format: primary_button table containing text_alignment (string),
//                  icon_alignment (string), icon_size (array of two integers),
//                  and policy (string)
[[nodiscard]] PrimaryButtonStyle primaryButtonStyle(
        const ConfigView& node, const PrimaryButtonStyle& defaults);

// Expected format: primary button table containing double_key_press (bool)
[[nodiscard]] PrimaryButtonBehavior primaryButtonBehavior(
        const ConfigView& node, const PrimaryButtonBehavior& defaults);

} // namespace config::map::helpers

#include "Config.tpp"
