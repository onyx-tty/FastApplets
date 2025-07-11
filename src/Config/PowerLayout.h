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

#ifndef POWER_LAYOUT_H
#define POWER_LAYOUT_H

#include "SharedLayout.h"
#include "../Applets/PowerApplet/Widgets/PowerButton.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QIcon>

#include <array>

namespace button {
extern std::array<PowerButton*, 4> list(QWidget* parent, QVBoxLayout* layout);
}

namespace style {
extern const QString selected;
extern const QString unselected;
}

namespace icon {
extern const QSize size;
extern QString location;
extern QIcon shutdown;
extern QIcon reboot;
extern QIcon suspend;
extern QIcon hibernate;
}

namespace text {
extern QString shutdown;
extern QString reboot;
extern QString suspend;
extern QString hibernate;
}

namespace policy {
extern const QSizePolicy buttons;
}

#endif // POWER_LAYOUT_H
