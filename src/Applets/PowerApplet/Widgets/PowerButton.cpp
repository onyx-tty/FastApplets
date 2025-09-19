/* Pretty Applets

   Copyright (C) 2024 Łukasz Wrodarczyk

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
#include "../DBus/PowerActionManager.h"

PowerButton::PowerButton(QWidget* parent, QHBoxLayout* main_layout, const QIcon button_icon,
                         const QString button_text, const QString dbus_action) :
        Button(parent, main_layout, button_icon, button_text), dbus_action(dbus_action) {
        connect(this, &PowerButton::clicked,
                [dbus_action]() { PowerActionManager::getInstance().sendPowerAction(dbus_action); });
}

const QString& PowerButton::getDBusAction() {
        return dbus_action;
}
