// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cassert>
#include <string_view>
#include <utility>

namespace applet {

enum class type { global, power_applet };

// Returns the name string for an applet::type
constexpr std::string_view toString(type applet_type) {
        switch (applet_type) {
        case type::global:       return "global";
        case type::power_applet: return "power_applet";
        // TODO: Replace assert() with qFatal()
        default:                 assert(false && "Passed unknown type"); std::unreachable();
        }
}

} // namespace applet
