// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLatin1StringView>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

namespace applet {

enum class type { global, power, action, player };

// Serializes string for applet::type.
[[nodiscard]] constexpr QStringView toString(type applet_type) {
        switch (applet_type) {
        case type::global: return u"global";
        case type::power:  return u"power_applet";
        case type::action: return u"action_applet";
        case type::player: return u"player_applet";
        default:           qFatal("Passed unknown type");
        }
}

// Serializes Latin1 string for applet::type.
[[nodiscard]] constexpr QLatin1StringView toLatin1String(type applet_type) {
        using namespace Qt::StringLiterals;

        switch (applet_type) {
        case type::global: return "global"_L1;
        case type::power:  return "power_applet"_L1;
        case type::action: return "action_applet"_L1;
        case type::player: return "player_applet"_L1;
        default:           qFatal("Passed unknown type");
        }
}

// Obtains title associated with applet::type.
[[nodiscard]] constexpr QStringView toTitle(type applet_type) {
        switch (applet_type) {
        case type::global: return u"FastApplets";
        case type::power:  return u"PowerApplet";
        case type::action: return u"ActionApplet";
        case type::player: return u"PlayerApplet";
        default:           qFatal("Passed unknown type");
        }
}

} // namespace applet
