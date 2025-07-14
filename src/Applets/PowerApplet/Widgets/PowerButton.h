/* Pretty Applets

   Copyright (C) 2025 Łukasz Wrodarczyk

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

#include "../../../Widgets/Button.h"

class PowerButton final : public Button {
        Q_OBJECT
        // TODO Construct and store full dbus action path for safety and flexibility
        const QString dbus_action;

public:
        PowerButton(QBoxLayout* const layout, const QIcon icon, const QString text,
                    const QString dbus_action);

        const QString& getDBusAction();
};
