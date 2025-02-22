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

// TODO Choosing from APIs on runtime

std::array<HourlyWeatherData, 39> debugInitHours();

class WeatherParser final {
public:
        explicit WeatherParser(QWidget* const parent, const QApplication* app,
                               const WeatherEnvProp& env_prop);

        void updateWeatherData();

private:
        /* Core */
        WeatherData    weather_data;
        /* APIs */
        OpenWeatherAPI open_weather;

        void traverseJson(
                const std::string& prime_key, const json& prime_value, std::string path,
                const std::function<void(const std::string&, const json&, std::string, int)>& handler,
                int& index);
        void processWeatherItem(const std::string& key, const json& value, const std::string& path,
                                int& index);
};

#endif // WEATHER_PARSER_H
