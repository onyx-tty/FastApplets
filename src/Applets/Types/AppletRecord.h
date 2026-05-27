// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletType.h"

#include <cassert>
#include <string_view>
#include <utility>

namespace applet {

// Stores all forms of applet representation in one record.
struct Record {
        applet::type     type;
        std::string_view name;
};

inline constexpr Record global{.type = type::global, .name = "global"};
inline constexpr Record power_applet{.type = type::power_applet, .name = "power_applet"};

// Returns the name string for an applet::type
constexpr std::string_view toString(type applet_type) {
        switch (applet_type) {
        case type::global:       return global.name;
        case type::power_applet: return power_applet.name;
        default:                 assert(false && "Passed unknown type"); std::unreachable();
        }
}

} // namespace applet
