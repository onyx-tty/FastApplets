// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLatin1StringView>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

namespace applet {

enum class Type { Global, Power, Action, Player };

// Serializes string for applet::Type.
[[nodiscard]] constexpr QStringView toString(Type applet_type) {
        switch (applet_type) {
        case Type::Global: return u"global";
        case Type::Power:  return u"power_applet";
        case Type::Action: return u"action_applet";
        case Type::Player: return u"player_applet";
        default:           qFatal("Passed unknown type");
        }
}

// Serializes Latin1 string for applet::Type.
[[nodiscard]] constexpr QLatin1StringView toLatin1String(Type applet_type) {
        using namespace Qt::StringLiterals;

        switch (applet_type) {
        case Type::Global: return "global"_L1;
        case Type::Power:  return "power_applet"_L1;
        case Type::Action: return "action_applet"_L1;
        case Type::Player: return "player_applet"_L1;
        default:           qFatal("Passed unknown type");
        }
}

// Obtains title associated with applet::Type.
[[nodiscard]] constexpr QStringView toTitle(Type applet_type) {
        switch (applet_type) {
        case Type::Global: return u"FastApplets";
        case Type::Power:  return u"PowerApplet";
        case Type::Action: return u"ActionApplet";
        case Type::Player: return u"PlayerApplet";
        default:           qFatal("Passed unknown type");
        }
}

} // namespace applet
