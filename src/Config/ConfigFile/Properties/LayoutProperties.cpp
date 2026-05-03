/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

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
