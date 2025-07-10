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

const QString shared_style::selected = "text-align: left; background-color: lightblue; border: 2px solid blue;";
const QString shared_style::unselected = "text-align: left;";

const QSize shared_icon::size(64, 64);

const QSizePolicy shared_policy::buttons(QSizePolicy::Expanding, QSizePolicy::Expanding);
