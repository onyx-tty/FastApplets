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

#ifndef WEATHER_PARSER_H
#define WEATHER_PARSER_H

#include "OpenWeather.h"
#include "WeatherData.h"

#include <QApplication>
#include <QWidget>

#include <string>
#include <utility>

// TODO Determining API of choice

class WeatherParser final {
public:
        explicit WeatherParser(QWidget* parent, QApplication* app, WeatherEnvProp& env_prop);

        void updateWeatherData();

private:
        OpenWeatherAPI   open_weather;
        /* Other APIs */
        DailyWeatherData weather_data;

        void     recursiveJsonIteration(std::pair<const std::string&, const json&> response,
                                        std::function<void(const json&, int)> parseItem);
        void     extractHourlyWeather(const json& chunk, int i);
        unsigned findClosestHour(); // TODO move elsewhere
};

#endif // WEATHER_PARSER_H
