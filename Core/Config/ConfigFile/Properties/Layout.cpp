// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Layout.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <utility>
#include <variant>
#include <vector>
#include <QIcon>
#include <QResource>
#include <QString>

/* PowerApplet */

template<>
QIcon iconFor<power_button_type>(power_button_type type) {
        using enum power_button_type;

        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case shutdown:  return QIcon{":/Icons/Power/shutdown.svg"};
        case reboot:    return QIcon{":/Icons/Power/reboot.svg"};
        case suspend:   return QIcon{":/Icons/Power/suspend.svg"};
        case hibernate: return QIcon{":/Icons/Power/hibernate.svg"};
        default:        return {};
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
        default:        return "";
        }
}

template<>
QString commandFor<power_button_type>(power_button_type type) {
        using enum power_button_type;

        switch (type) {
        case shutdown:  return "systemctl poweroff";
        case reboot:    return "systemctl reboot";
        case suspend:   return "systemctl suspend";
        case hibernate: return "systemctl hibernate";
        default:        return "";
        }
}

/* VolumeApplet */

template<>
QIcon iconFor<volume_button_type>(volume_button_type type) {
        using enum volume_button_type;

        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case play_pause:         return {};
        case next:               return {};
        case previous:           return {};
        case volume_up:          return {};
        case volume_down:        return {};
        case volume_mute_output: return {};
        case volume_mute_input:  return {};
        default:                 return {};
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
        default:                 return {};
        }
}

template<>
QString commandFor<volume_button_type>(volume_button_type type) {
        using enum volume_button_type;

        switch (type) {
        case play_pause:         return "playerctl play-pause";
        case next:               return "playerctl next";
        case previous:           return "playerctl previous";
        case volume_up:          return "playerctl volume 0.1+";
        case volume_down:        return "playerctl volume 0.1-";
        case volume_mute_output: return "noctalia msg volume-mute";
        default:                 return {};
        }
}

/* ActionApplet */

template<>
QIcon iconFor<std::monostate>(std::monostate type) {
        return {};
}

template<>
QString textFor<std::monostate>(std::monostate type) {
        return {};
}

template<>
QString commandFor<std::monostate>(std::monostate type) {
        return {};
}

using Layout = config::schema::properties::Layout;

/* Layout Properties */

Layout::Layout(std::vector<PrimaryButtonParams> primary_buttons) :
        primary_buttons(std::move(primary_buttons)) {}

const std::vector<PrimaryButtonParams>& Layout::getPrimaryButtons() const {
        return primary_buttons;
}
