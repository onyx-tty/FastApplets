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
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Log/Log.h"
#include "Shell/ShellCommand.h"
#include "Shell/ShellRunner.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PrimaryButton.h"

#include <qobject.h>
#include <QBoxLayout>
#include <QIcon>
#include <QString>

PowerButton::PowerButton(power_button_id id, const QIcon& icon, const QString& text,
                         const QString& dbus_action, const ShellCommand& command) :
        PrimaryButton(icon, text, PowerAppletConfig::get().getPrimaryButtonProperties()), id(id),
        dbus_action(dbus_action) {
        connect(this, &PowerButton::clicked,
                [this, command]() { ShellRunner::runCommand(command); });

        QDEBUG_NS() << "Created " << text << "!";
}

power_button_id PowerButton::getID() const {
        return id;
}

const QString& PowerButton::getDBusAction() const {
        return dbus_action;
}
