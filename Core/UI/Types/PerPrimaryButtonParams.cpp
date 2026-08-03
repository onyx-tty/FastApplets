// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PerPrimaryButtonParams.h"
#include "Core/UI/Types/ButtonType.h"

#include <variant>
#include <QIcon>
#include <QResource>
#include <QString>

/* PowerApplet */

template<>
QIcon iconFor<power_button_type>(power_button_type type) {
        using enum power_button_type;

        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case shutdown:  return QIcon(":/Icons/Power/shutdown.svg");
        case reboot:    return QIcon(":/Icons/Power/reboot.svg");
        case suspend:   return QIcon(":/Icons/Power/suspend.svg");
        case hibernate: return QIcon(":/Icons/Power/hibernate.svg");
        case log_out:   return QIcon(":/Icons/Power/log_out.svg");
        default:        return QIcon(":/Icons/missing.svg");
        }
}

template<>
QString textFor<power_button_type>(power_button_type type) {
        using enum power_button_type;

        switch (type) {
        case shutdown:  return "Shutdown";
        case reboot:    return "Reboot";
        case suspend:   return "Suspend";
        case hibernate: return "Hibernate";
        case log_out:   return "Log Out";
        default:        return "";
        }
}

template<>
QString commandFor<power_button_type>(power_button_type type) {
        using enum power_button_type;

        // clang-format off
        switch (type) {
        case shutdown:  return "systemctl poweroff";
        case reboot:    return "systemctl reboot";
        case suspend:   return "systemctl suspend";
        case hibernate: return "systemctl hibernate";
        case log_out:   return "loginctl terminate-session"
                               "$(loginctl session-status | head -1 | awk '{print $1}')";
        default: return "";
        }
        // clang-format on
}

/* PlayerApplet */

template<>
QIcon iconFor<volume_button_type>(volume_button_type type) {
        using enum volume_button_type;

        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case play_pause:         return QIcon(":/Icons/Player/play.svg");
        case next:               return QIcon(":/Icons/Player/next.svg");
        case previous:           return QIcon(":/Icons/Player/back.svg");
        case volume_up:          return QIcon(":/Icons/Player/volume3.svg");
        case volume_down:        return QIcon(":/Icons/Player/volume1.svg");
        case volume_mute_output: return QIcon(":/Icons/Player/mute.svg");
        case volume_mute_input:  return QIcon(":/Icons/Player/microphone.svg");
        default:                 return QIcon(":/Icons/missing.svg");
        }
}

template<>
QString textFor<volume_button_type>(volume_button_type type) {
        using enum volume_button_type;

        switch (type) {
        case play_pause:         return "Play/Pause";
        case next:               return "Next";
        case previous:           return "Previous";
        case volume_up:          return "Volume Up";
        case volume_down:        return "Volume Down";
        case volume_mute_output: return "Mute Output";
        case volume_mute_input:  return "Mute Input";
        default:                 return "";
        }
}

template<>
QString commandFor<volume_button_type>(volume_button_type type) {
        using enum volume_button_type;

        switch (type) {
        case play_pause:  return "playerctl play-pause";
        case next:        return "playerctl next";
        case previous:    return "playerctl previous";
        case volume_up:   return "playerctl volume 0.1+";
        case volume_down: return "playerctl volume 0.1-";
        default:          return "";
        }
}

/* ActionApplet */

template<>
QIcon iconFor<std::monostate>(std::monostate type) {
        Q_INIT_RESOURCE(Icons);

        return QIcon(":/Icons/missing.svg");
}

template<>
QString textFor<std::monostate>(std::monostate type) {
        return {};
}

template<>
QString commandFor<std::monostate>(std::monostate type) {
        return {};
}
