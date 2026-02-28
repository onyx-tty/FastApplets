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

#include "PowerButton.h"
#include "Core/Log.h"
#include "DBus/PowerActionManager.h"
#include "Environment/ShellRunner.h"

PowerButton::PowerButton(QBoxLayout* layout, QIcon icon, QString text, QString dbus_action) :
        Button(layout, icon, text), dbus_action(dbus_action) {
        /*
        connect(this, &PowerButton::clicked,
                [this]() { PowerActionManager::sendPowerAction(this->dbus_action); });
        */
        if (dbus_action == "PowerOff") {
                connect(this, &PowerButton::clicked,
                        [this]() { ShellRunner::runShutdownCommand(); });
        } else if (dbus_action == "Reboot") {
                connect(this, &PowerButton::clicked, [this]() { ShellRunner::runRebootCommand(); });
        } else if (dbus_action == "Suspend") {
                connect(this, &PowerButton::clicked, [this]() { ShellRunner::runSuspendCommand(); });
        } else if (dbus_action == "Hibernate") {
                connect(this, &PowerButton::clicked,
                        [this]() { ShellRunner::runHibernateCommand(); });
        } else {
                QCRITICAL() << "Wrong command! PowerButton" << text
                            << "declared with dbus_action =" << dbus_action;
        }
}

const QString& PowerButton::getDBusAction() {
        return dbus_action;
}
