// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <optional>
#include <toml++/toml.hpp>
#include <vector>

class QString;
class QSize;
class QSizePolicy;

namespace config::map::helpers {

using config::CandidateIndex;
using config::Candidates;

/* WindowParams */

// Maps WindowParams from config nodes.
//
// Fallback priority: applet > global > hardcoded defaults
//
// Expected format: window table containing title (string) and
//                  size (array of two integers)
//
// Return value: WindowParams
[[nodiscard]] WindowParams windowParams(const Candidates& candidates, const WindowParams& defaults);

/* PrimaryButtonParams */

// Maps PrimaryButtonParams from a list of candidates.
//
// Expected format: primary_button table containing double_key_press (bool),
//                  text_alignment (string), icon_alignment (string),
//                  icon_size (array of two integers), policy (string), and
//                  list (array of tables)
//
// Return value: PrimaryButtonParams
template<applet::Type TApplet>
[[nodiscard]] PrimaryButtonParams primaryButtonParams(const Candidates&          candidates,
                                                      const PrimaryButtonParams& defaults);

// Maps std::vector<PerPrimaryButtonParams> from a list of candidates.
//
// Expected format: array of tables
//
// Return value: std::vector<PerPrimaryButtonParams>
template<applet::Type TApplet>
[[nodiscard]] std::vector<PerPrimaryButtonParams> perPrimaryButtonParamsList(
        const Candidates& candidates, const std::vector<PerPrimaryButtonParams>& defaults);

// Maps PerPrimaryButtonParams from a list of candidates.
//
// Expected format: primary_buttons.list[index] table containing type (string),
//                  text (string), and command (string)
//
// Regarding PerPrimaryButtonParams::command: QProcess::splitCommand() does not
// interpret single quotes as quotes, but as regular characters. This function
// converts single quotes to double quotes to work around that limitation.
//
// Return value: std::optional<PerPrimaryButtonParams>
template<applet::Type TApplet>
[[nodiscard]] std::optional<PerPrimaryButtonParams> perPrimaryButtonParams(
        const Candidates& candidates);

// Maps PrimaryButtonStyle from a list of candidates.
//
// Expected format: primary_button table containing text_alignment (string),
//                  icon_alignment (string), icon_size (array of two integers),
//                  and policy (string)
//
// Return value: PrimaryButtonStyle
[[nodiscard]] PrimaryButtonStyle primaryButtonStyle(const Candidates&         candidates,
                                                    const PrimaryButtonStyle& defaults);

// Maps PrimaryButtonBehavior from a list of candidates.
//
// Expected format: primary button table containing double_key_press (bool).
//
// Return value: PrimaryButtonBehavior
[[nodiscard]] PrimaryButtonBehavior primaryButtonBehavior(const Candidates&            candidates,
                                                          const PrimaryButtonBehavior& defaults);

} // namespace config::map::helpers

#include "Config.tpp"
