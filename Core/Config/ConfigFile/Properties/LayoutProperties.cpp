// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LayoutProperties.h"
#include "Core/UI/Types/ButtonType.h"

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
