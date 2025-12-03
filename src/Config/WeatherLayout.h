/* FastApplets

   Copyright (C) 2025 Łukasz Wrodarczyk

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

#pragma once

#include "../Applets/WeatherApplet/Data/WeatherData.h"
#include "SharedLayout.h"

#include <QSize>
#include <QString>

#include <string>
#include <unordered_map>

struct WeatherMainWindowProp : public MainWindowProp {
        const QSize   size;
        const QString title;

        explicit WeatherMainWindowProp(const QSize size, const QString title);
};

struct WeatherStyleProp : public StyleProp {
        const QString button_stylesheet;

        explicit WeatherStyleProp(const QString button_stylesheet);
};

struct WeatherEnvProp : public EnvProp {
        explicit WeatherEnvProp(QString project_root_marker);

        static std::string getOpenWeatherKey();
        static std::string getAPICallURL(); // uses getOpenWeatherKey
};

struct WeatherLayoutProp : public LayoutProp {
        const QSizePolicy                                      cell_policy;
        static const std::unordered_map<int, WeatherCondition> weather_list;

        explicit WeatherLayoutProp(const QSizePolicy button_policy, const QSizePolicy cell_policy);
};

// TODO Optimize to avoid repeating identical props
struct WeatherLayoutManager : public LayoutManager {
public:
        static const WeatherMainWindowProp main_window_prop;
        static const WeatherStyleProp      style_prop;
        // TODO WeatherButtonProp
        static const WeatherEnvProp        env_prop;
        static const WeatherLayoutProp     layout_prop;

        explicit WeatherLayoutManager() = delete;
};
