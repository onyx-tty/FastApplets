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

#include "SharedLayout.h"

#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QString>

// main window
QSize         shared_main_window::size(600, 300);
const QString shared_main_window::title = "test_window";

// style
const QString shared_style::selected =
        "text-align: center top; background-color: lightblue; border: 2px solid blue;";
const QString shared_style::unselected = "text-align: center top;";

// icons
const QSize shared_icon::size(64, 64);

// button alignment
const Qt::Alignment shared_button_alignment::icon(Qt::AlignHCenter | Qt::AlignBottom);
const Qt::Alignment shared_button_alignment::text(Qt::AlignHCenter | Qt::AlignTop);

// policies
const QSizePolicy shared_policy::buttons(QSizePolicy::Expanding, QSizePolicy::Expanding);
