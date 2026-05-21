// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletType.h"

#include <string_view>

namespace applet {

// Stores all forms of applet representation in one record
struct Record {
        applet::type     id;
        std::string_view scope;
};

inline constexpr Record global{.id = type::global, .scope = "global"};
inline constexpr Record power_applet{.id = type::power_applet, .scope = "power_applet"};

// Returns the scope string for an applet::type
constexpr std::string_view toString(applet::type id) {
        switch (id) {
        case type::global:       return global.scope;
        case type::power_applet: return power_applet.scope;
        }
}

} // namespace applet
