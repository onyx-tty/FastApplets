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

#include "PowerLayout.h"

// Modify these as much as you want to customize the appearance

QString icon_location = "Data/";

// button order
std::array<PowerButton *, 4> button::list(QWidget *parent, QVBoxLayout *layout) {
        std::array<PowerButton *, 4> button_list = {
                new PowerButton(parent, layout, icon::shutdown, text::shutdown),
                new PowerButton(parent, layout, icon::reboot, text::reboot),
                new PowerButton(parent, layout, icon::suspend, text::suspend),
                new PowerButton(parent, layout, icon::hibernate, text::hibernate),
        };
        return button_list;
}

// icon
const QSize icon::size = shared_icon::size;
QString icon::location(icon_location);
QIcon icon::shutdown(icon_location + "shutdown.svg");
QIcon icon::reboot(icon_location + "reboot.svg");
QIcon icon::suspend(icon_location + "suspend.svg");
QIcon icon::hibernate(icon_location + "hibernate.svg");

// text
QString text::shutdown("Shutdown");
QString text::reboot("Reboot");
QString text::suspend("Suspend");
QString text::hibernate("Hibernate");

// policy
const QSizePolicy policy::buttons = shared_policy::buttons;
