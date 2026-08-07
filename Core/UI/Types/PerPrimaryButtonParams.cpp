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
QIcon iconFor<PowerButtonType>(PowerButtonType type) {
        using enum PowerButtonType;

        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case Shutdown:  return QIcon(":/Icons/Power/shutdown.svg");
        case Reboot:    return QIcon(":/Icons/Power/reboot.svg");
        case Suspend:   return QIcon(":/Icons/Power/suspend.svg");
        case Hibernate: return QIcon(":/Icons/Power/hibernate.svg");
        case LogOut:    return QIcon(":/Icons/Power/log_out.svg");
        default:        return QIcon(":/Icons/missing.svg");
        }
}

template<>
QString textFor<PowerButtonType>(PowerButtonType type) {
        using enum PowerButtonType;

        switch (type) {
        case Shutdown:  return "Shutdown";
        case Reboot:    return "Reboot";
        case Suspend:   return "Suspend";
        case Hibernate: return "Hibernate";
        case LogOut:    return "Log Out";
        default:        return "";
        }
}

template<>
QString commandFor<PowerButtonType>(PowerButtonType type) {
        using enum PowerButtonType;

        // clang-format off
        switch (type) {
        case Shutdown:  return "systemctl poweroff";
        case Reboot:    return "systemctl reboot";
        case Suspend:   return "systemctl suspend";
        case Hibernate: return "systemctl hibernate";
        case LogOut:   return "loginctl terminate-session"
                               "$(loginctl session-status | head -1 | awk '{print $1}')";
        default: return "";
        }
        // clang-format on
}

/* PlayerApplet */

template<>
QIcon iconFor<VolumeButtonType>(VolumeButtonType type) {
        using enum VolumeButtonType;

        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case PlayPause:        return QIcon(":/Icons/Player/play.svg");
        case Next:             return QIcon(":/Icons/Player/next.svg");
        case Previous:         return QIcon(":/Icons/Player/back.svg");
        case VolumeUp:         return QIcon(":/Icons/Player/volume3.svg");
        case VolumeDown:       return QIcon(":/Icons/Player/volume1.svg");
        case VolumeMuteOutput: return QIcon(":/Icons/Player/mute.svg");
        case VolumeMuteInput:  return QIcon(":/Icons/Player/microphone.svg");
        default:               return QIcon(":/Icons/missing.svg");
        }
}

template<>
QString textFor<VolumeButtonType>(VolumeButtonType type) {
        using enum VolumeButtonType;

        switch (type) {
        case PlayPause:        return "Play/Pause";
        case Next:             return "Next";
        case Previous:         return "Previous";
        case VolumeUp:         return "Volume Up";
        case VolumeDown:       return "Volume Down";
        case VolumeMuteOutput: return "Mute Output";
        case VolumeMuteInput:  return "Mute Input";
        default:               return "";
        }
}

template<>
QString commandFor<VolumeButtonType>(VolumeButtonType type) {
        using enum VolumeButtonType;

        switch (type) {
        case PlayPause:  return "playerctl play-pause";
        case Next:       return "playerctl next";
        case Previous:   return "playerctl previous";
        case VolumeUp:   return "playerctl volume 0.1+";
        case VolumeDown: return "playerctl volume 0.1-";
        default:         return "";
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
