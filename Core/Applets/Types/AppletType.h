// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cassert>
#include <utility>
#include <QLatin1StringView>
#include <QStringView>
#include <Qt>

namespace applet {

enum class type { global, power_applet, action_applet };

// Serializes string for applet::type.
constexpr QStringView toString(type applet_type) {
        switch (applet_type) {
        case type::global:        return u"global";
        case type::power_applet:  return u"power_applet";
        case type::action_applet: return u"action_applet";
        // TODO: Replace assert() with qFatal()
        default:                  assert(false && "Passed unknown type"); std::unreachable();
        }
}

// Serializes Latin1 string for applet::type.
constexpr QLatin1StringView toLatin1String(type applet_type) {
        using namespace Qt::StringLiterals;

        switch (applet_type) {
        case type::global:        return "global"_L1;
        case type::power_applet:  return "power_applet"_L1;
        case type::action_applet: return "action_applet"_L1;
        // TODO: Replace assert() with qFatal()
        default:                  assert(false && "Passed unknown type"); std::unreachable();
        }
}

} // namespace applet
