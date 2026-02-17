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

#include "Config.h"

using std::vector;

/* Window Properties*/
const QString& Config::WindowProperties::getTitle() {
        return Config::WindowProperties::title;
}

const QSize& Config::WindowProperties::getSize() {
        return Config::WindowProperties::size;
}

/* Window Layout Properties */
const vector<PrimaryButtonData>& Config::WindowLayoutProperties::getPrimaryPowerButtons() {
        return Config::WindowLayoutProperties::primary_power_buttons;
}

/* Primary Button Properties */
const Qt::Alignment& Config::PrimaryButtonProperties::getTextAlignment() {
        return Config::PrimaryButtonProperties::text_alignment;
}

const Qt::Alignment& Config::PrimaryButtonProperties::getIconAlignment() {
        return Config::PrimaryButtonProperties::icon_alignment;
}

const QSize& Config::PrimaryButtonProperties::getIconSize() {
        return Config::PrimaryButtonProperties::icon_size;
}

const QSizePolicy& Config::PrimaryButtonProperties::getPolicy() {
        return Config::PrimaryButtonProperties::policy;
}

