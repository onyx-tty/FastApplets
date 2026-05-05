// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LayoutProperties.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <utility>
#include <vector>
#include <QIcon>
#include <QResource>
#include <QString>

QIcon iconFor(power_button_id id) {
        Q_INIT_RESOURCE(Icons);

        switch (id) {
        case power_button_id::shutdown:  return QIcon{":/Icons/Power/shutdown.svg"};
        case power_button_id::reboot:    return QIcon{":/Icons/Power/reboot.svg"};
        case power_button_id::suspend:   return QIcon{":/Icons/Power/suspend.svg"};
        case power_button_id::hibernate: return QIcon{":/Icons/Power/hibernate.svg"};
        }

        return {};
}

QString dbusMethodFor(power_button_id id) {
        switch (id) {
        case power_button_id::shutdown:  return "PowerOff";
        case power_button_id::reboot:    return "Reboot";
        case power_button_id::suspend:   return "Suspend";
        case power_button_id::hibernate: return "Hibernate";
        }

        QCRITICAL() << "DBus method unclear, power_button_id:" << static_cast<int>(id);
        return {};
}

LayoutProperties::LayoutProperties(std::vector<PowerButtonParams> power_buttons) :
        power_buttons(std::move(power_buttons)) {}

const std::vector<PowerButtonParams>& LayoutProperties::getPowerButtons() const {
        return power_buttons;
}
