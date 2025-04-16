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

#include "../Applets/WeatherApplet/API/WeatherData.h"
#include "SharedLayout.h"

#include <QSize>
#include <QString>

#include <string>
#include <unordered_map>

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
        // TODO Rewrite to include other weather APIs
        std::string& getOpenWeatherKey() const;

        explicit WeatherEnvProp(const QApplication* app);
};

struct WeatherLayoutProp : public LayoutProp {
        static const std::unordered_map<int, WeatherCondition> weather_list;

        explicit WeatherLayoutProp();
};

struct WeatherLayoutManager : public LayoutManager {
        static WeatherMainWindowProp main_window_prop;
        static WeatherStyleProp      style_prop;
        static WeatherLayoutProp     layout_prop;

        static const WeatherEnvProp& getEnvProp(const QApplication* app);

        explicit WeatherLayoutManager();
};

#endif // WEATHER_LAYOUT_H
