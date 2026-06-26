// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <variant>

class QStringView;

// 'none' represents empty state, e.g. no button selected, if used for that purpose
enum class power_button_type { none, shutdown, reboot, suspend, hibernate };

template<typename TPrimaryButtonType>
TPrimaryButtonType toPrimaryButtonType(QStringView string);

// Button type is only needed if given applet can predict what the user needs.
// For instance, the user of PowerApplet will likely need shutdown and reboot,
// so suitable defaults can be provided, but ActionApplet is too flexible
// for that behavior and lacks a strict predetermined purpose, aside assigning
// shell commands to button clicks.
//
// This is essentially an alternative to making button type-dependent classes generic,
// as Q_OBJECT doesn't support templated classes. It also avoids an unnecessary
// inheritance hierarchy.
using ButtonType = std::optional<std::variant<power_button_type>>;
