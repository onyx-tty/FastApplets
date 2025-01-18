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

enum class WeatherIcon {
        clear,
        few_clouds,
        scattered_clouds,
        broken_clouds,
        shower_rain,
        rain,
        thunderstorm,
        snow,
        mist,
};

struct WeatherCondition final {
        std::string name;
        std::string detailed_name;
        QImage      day_icon;
        QImage      night_icon;

        WeatherCondition(std::string name, std::string detailed_name, QImage day_icon,
                         QImage night_icon);
        WeatherCondition(const WeatherCondition& other);
        WeatherCondition& operator=(const WeatherCondition& other);
        WeatherCondition(WeatherCondition&& other);
        WeatherCondition& operator=(WeatherCondition&& other);
};

struct HourlyWeatherData final {
        std::time_t                                           time;
        WeatherCondition&                                     weather;
        float                                                 temperature;
        float                                                 temperature_min;
        float                                                 temperature_max;
        unsigned                                              atmospheric_pressure;
        unsigned                                              rain;
        unsigned                                              humidity;
        unsigned                                              wind_speed;
        static std::unordered_map<unsigned, WeatherCondition> weathers;

        // TODO Alias for all the repeating arguments
        HourlyWeatherData(decltype(time) time, decltype(weather) weather,
                          decltype(temperature)          temperature,
                          decltype(temperature_min)      temperature_min,
                          decltype(temperature_max)      temperature_max,
                          decltype(atmospheric_pressure) atmospheric_pressure, decltype(rain) rain,
                          decltype(humidity) humidity, decltype(wind_speed) wind_speed);

        HourlyWeatherData(const HourlyWeatherData& other);
        HourlyWeatherData& operator=(const HourlyWeatherData& other);
        HourlyWeatherData(HourlyWeatherData&& other);
        HourlyWeatherData& operator=(HourlyWeatherData&& other);

        void setWeatherData(decltype(time) time, decltype(weather) weather,
                            decltype(temperature)          temperature,
                            decltype(temperature_min)      temperature_min,
                            decltype(temperature_max)      temperature_max,
                            decltype(atmospheric_pressure) atmospheric_pressure,
                            decltype(rain) rain, decltype(humidity) humidity,
                            decltype(wind_speed) wind_speed);
};

struct DailyWeatherData final {
        std::array<HourlyWeatherData, 8> hours;
        float                            min_temperature;
        float                            max_temperature;

        void setWeatherData(decltype(hours), decltype(min_temperature), decltype(max_temperature));
        void setTemperatureRange();

        DailyWeatherData();
};

#endif // WEATHER_DATA_H
