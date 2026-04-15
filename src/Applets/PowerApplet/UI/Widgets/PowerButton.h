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

#pragma once

#include "Shell/ShellCommand.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PrimaryButton.h"

#include <QBoxLayout>
#include <QIcon>

class QString;

class PowerButton final : public PrimaryButton {
        Q_OBJECT

private:
        const power_button_id id;
        const QString         dbus_action;

public:
        explicit PowerButton(power_button_id id, const QIcon& icon, const QString& label,
                             const QString& dbus_action, const ShellCommand& command);
        power_button_id getID() const;
        const QString&  getDBusAction() const;
};
