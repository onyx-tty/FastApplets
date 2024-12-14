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

#ifndef WEATHER_LAYOUT_H
#define WEATHER_LAYOUT_H

#include "SharedLayout.h"

struct WeatherMainWindowProp : public MainWindowProp {
        const QSize   size;
        const QString title;

        explicit WeatherMainWindowProp();
};

struct WeatherStyleProp : public StyleProp {
        const QString selected;
        const QString unselected;

        explicit WeatherStyleProp();
};

struct WeatherEnvProp : public EnvProp {
        explicit WeatherEnvProp();
};

class WeatherLayoutManager {
        static WeatherMainWindowProp main_window_prop;
        static WeatherStyleProp      style_prop;

        explicit WeatherLayoutManager();
};

#endif // WEATHER_LAYOUT_H
