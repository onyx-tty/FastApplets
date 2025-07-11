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

/* Modify to adjust application style */

// window properties
QSize         main_window::size  = QSize(960, 200);
const QString main_window::title = shared_main_window::title;

QString icon_location = "Data/";

// button order
std::array<PowerButton *, 4> button::list(QWidget *parent, QHBoxLayout *layout) {
        std::array<QIcon, 4>         icon_list   = icon::initIcons();
        std::array<PowerButton *, 4> button_list = {
                new PowerButton(parent, layout, icon_list[0], text::shutdown, "PowerOff"),
                new PowerButton(parent, layout, icon_list[1], text::reboot, "Reboot"),
                new PowerButton(parent, layout, icon_list[2], text::suspend, "Suspend"),
                new PowerButton(parent, layout, icon_list[3], text::hibernate, "Hibernate"),
        };
        return button_list;
}

// icons
std::array<QIcon, 4> icon::initIcons() {
        return std::array<QIcon, 4>{
                QIcon(":/Icons/shutdown.svg"),
                QIcon(":/Icons/reboot.svg"),
                QIcon(":/Icons/suspend.svg"),
                QIcon(":/Icons/hibernate.svg"),
        };
}

// styles
const QString style::selected   = shared_style::selected;
const QString style::unselected = shared_style::unselected;

// button alignment
const Qt::Alignment button_alignment::icon = shared_button_alignment::icon;
const Qt::Alignment button_alignment::text = shared_button_alignment::text;

// text
QString text::shutdown("Shutdown");
QString text::reboot("Reboot");
QString text::suspend("Suspend");
QString text::hibernate("Hibernate");

// policy
const QSizePolicy policy::buttons = shared_policy::buttons;
