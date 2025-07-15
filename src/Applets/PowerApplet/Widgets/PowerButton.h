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

#ifndef POWER_BUTTON_H
#define POWER_BUTTON_H

#include "../../../Widgets/Button.h"

class PowerButton final : public Button {
        Q_OBJECT

public:
        PowerButton(QWidget* parent, QHBoxLayout* main_layout, const QIcon button_icon,
                    const QString button_text, const QString action);
};

#endif // POWER_BUTTON_H
