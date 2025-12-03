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

#include <QImage>
#include <QString>

#include <array>
#include <ctime>
#include <optional>

constexpr int hourly_weather_data_hours = 40;

struct WeatherCondition final {
        QString name;
        QString detailed_name;
        QImage  day_icon;
        QImage  night_icon;

        WeatherCondition(QString name, QString detailed_name, const QImage& day_icon,
                         const QImage& night_icon);
        WeatherCondition(const WeatherCondition& other);
        WeatherCondition& operator=(const WeatherCondition& other);
        WeatherCondition(WeatherCondition&& other);
        WeatherCondition& operator=(WeatherCondition&& other);

        QString getData() const;
};

struct HourlyWeatherData final {
        QString                 day;
        time_t                  time;
        QString                 city_name;
        const WeatherCondition* weather;
        float                   temperature;
        float                   temperature_feels_like;
        float                   temperature_min;
        float                   temperature_max;
        int                     atmospheric_pressure;
        float                   rain;
        int                     humidity;
        float                   wind_speed;

        HourlyWeatherData();

        HourlyWeatherData(const HourlyWeatherData& other);
        HourlyWeatherData& operator=(const HourlyWeatherData& other);
        HourlyWeatherData(HourlyWeatherData&& other);
        HourlyWeatherData& operator=(HourlyWeatherData&& other);

        void printData() const;
};

class WeatherData final {
public:
        // TODO Private but easy and free initialization
        // TODO Type alias file
        static std::array<HourlyWeatherData, hourly_weather_data_hours> hours;

        WeatherData() = delete;

        static void printData();
        static void fillDayNames(const int&                      blocs_per_day,
                                 const std::optional<const int>& first_day_blocs);
};
