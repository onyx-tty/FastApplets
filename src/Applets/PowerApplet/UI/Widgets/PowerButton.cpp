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
#include "Applets/PowerApplet/DBus/PowerActionManager.h"
#include "Config/Config/PowerApplet/PowerAppletConfig.h"
#include "Core/Log.h"
#include "Environment/ShellCommand.h"
#include "Environment/ShellRunner.h"
#include "UI/Enums/ButtonIDs.h"

PowerButton::PowerButton(QBoxLayout* layout, power_button_id id, QIcon icon, QString text,
                         QString dbus_action, ShellCommand command) :
        Button(layout, icon, text,
               PowerAppletConfig::getPowerAppletConfig().getPrimaryButtonProperties()),
        id(id), dbus_action(dbus_action) {
        connect(this, &PowerButton::clicked,
                [this, command]() { ShellRunner::runCommand(command); });
}

power_button_id PowerButton::getID() const {
        return id;
}

const QString& PowerButton::getDBusAction() const {
        return dbus_action;
}
