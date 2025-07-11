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

#ifndef SHARED_LAYOUT_H
#define SHARED_LAYOUT_H

#include <QIcon>
#include <QSize>
#include <QSizePolicy>

// TODO Consider turning these into one manager struct for inheritance

namespace shared_main_window {
extern QSize size;
extern const QString title;
}

namespace shared_style {
extern const QString selected;
extern const QString unselected;
}

namespace shared_icon {
extern const QSize size;
}

namespace shared_button_alignment {
extern const Qt::Alignment icon;
extern const Qt::Alignment text;
}

namespace shared_policy {
extern const QSizePolicy buttons;
}

#endif // SHARED_LAYOUT_H
