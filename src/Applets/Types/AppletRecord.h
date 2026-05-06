// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletTypes.h"

#include <string_view>

namespace applet {

struct Record {
        applet::type     id;
        std::string_view scope;
};

inline constexpr Record global{type::global, "global"};
inline constexpr Record power_applet{type::power_applet, "power_applet"};

} // namespace applet
