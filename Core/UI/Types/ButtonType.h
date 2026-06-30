// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <type_traits>
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
using button_type = std::variant<std::monostate, power_button_type>;

// Returns true if std::get on button_type is ::none, otherwise false.
// Returns false if enum value in the button_type is not ::none, or if there's no way to obtain
// an enum value, for example because passed type is std::monostate.
template<typename TButtonType>
inline bool isNone(button_type type) {
        // Returns false if template param is std::monostate because that means the caller doesn't
        // want to extract an enum.
        if constexpr (std::is_same_v<TButtonType, std::monostate>) {
                return false;
        } else {
                // If button_type is already set to std::monostate then there's no enum to enum
                // to compare to.
                if (std::holds_alternative<std::monostate>(type)) { return false; }

                return static_cast<int>(std::get<TButtonType>(type)) == 0;
        }
}
