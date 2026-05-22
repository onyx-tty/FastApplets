// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LayoutProperties.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <utility>
#include <vector>
#include <QIcon>
#include <QResource>
#include <QString>

QIcon iconFor(power_button_type type) {
        using enum power_button_type;
        Q_INIT_RESOURCE(Icons);

        switch (type) {
        case shutdown:  return QIcon{":/Icons/Power/shutdown.svg"};
        case reboot:    return QIcon{":/Icons/Power/reboot.svg"};
        case suspend:   return QIcon{":/Icons/Power/suspend.svg"};
        case hibernate: return QIcon{":/Icons/Power/hibernate.svg"};
        }

        return {};
}

QString textFor(power_button_type type) {
        using enum power_button_type;

        switch (type) {
        case shutdown:  return "Shutdown";
        case reboot:    return "Reboot";
        case suspend:   return "Suspend";
        case hibernate: return "Hibernate";
        }

        return "";
}

QString commandFor(power_button_type type) {
        using enum power_button_type;

        switch (type) {
        case shutdown:  return "systemctl poweroff";
        case reboot:    return "systemctl reboot";
        case suspend:   return "systemctl suspend";
        case hibernate: return "systemctl hibernate";
        }

        return "";
}

LayoutProperties::LayoutProperties(std::vector<PowerButtonParams> power_buttons) :
        power_buttons(std::move(power_buttons)) {}

const std::vector<PowerButtonParams>& LayoutProperties::getPowerButtons() const {
        return power_buttons;
}
