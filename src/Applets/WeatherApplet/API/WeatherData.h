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

#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

#include <QImage>

#include <array>
#include <ctime>
#include <string>
#include <unordered_map>

struct WeatherCondition final {
        std::string name;
        std::string detailed_name;
        QImage      day_icon;
        QImage      night_icon;

        WeatherCondition(std::string name, std::string detailed_name, const QImage& day_icon,
                         const QImage& night_icon);
        WeatherCondition(const WeatherCondition& other);
        WeatherCondition& operator=(const WeatherCondition& other);
        WeatherCondition(WeatherCondition&& other);
        WeatherCondition& operator=(WeatherCondition&& other);

        QString getData() const;
};

struct HourlyWeatherData final {
        std::time_t             time;
        const WeatherCondition* weather;
        float                   temperature;
        float                   temperature_feels_like;
        float                   temperature_min;
        float                   temperature_max;
        int                     atmospheric_pressure;
        float                   rain;
        int                     humidity;
        float                   wind_speed;

        // TODO Alias for all the repeating arguments
        HourlyWeatherData(const WeatherCondition& default_weather);

        HourlyWeatherData(const HourlyWeatherData& other);
        HourlyWeatherData& operator=(const HourlyWeatherData& other);
        HourlyWeatherData(HourlyWeatherData&& other);
        HourlyWeatherData& operator=(HourlyWeatherData&& other);

        void printData() const;
};

class WeatherData final {
private:
        std::array<HourlyWeatherData, 39> hours;

public:
        static const std::unordered_map<int, WeatherCondition> weathers;

        WeatherData(const std::array<HourlyWeatherData, 39> hours);

        std::array<HourlyWeatherData, 39>& getHours();
        void                               printData() const;
};

#endif // WEATHER_DATA_H
