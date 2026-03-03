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

#include "UI/Widgets/Button.h"

class PowerButton final : public Button {
        Q_OBJECT

private:
        const QString identifier;
        const QString dbus_action;

public:
        explicit PowerButton(QBoxLayout* layout, QString identifier, QIcon icon, QString text,
                             QString dbus_action);

        const QString& getIdentifier() const;
        const QString& getDBusAction() const;
};
