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

#include "WeatherData.h"

#include <QDebug>

WeatherCondition::WeatherCondition(std::string name, std::string detailed_name,
                                   const QImage& day_icon, const QImage& night_icon) :
        name(std::move(name)), detailed_name(std::move(detailed_name)), day_icon(day_icon),
        night_icon(night_icon) {
        qInfo() << "WeatherCondition registered:" << name << detailed_name << day_icon
                << night_icon;
}

WeatherCondition::WeatherCondition(const WeatherCondition& other) :
        name(other.name), detailed_name(other.detailed_name), day_icon(other.day_icon),
        night_icon(other.night_icon) {}

WeatherCondition& WeatherCondition::operator=(const WeatherCondition& other) {
        if (this != &other) {
                name          = other.name;
                detailed_name = other.detailed_name;
                day_icon      = other.day_icon;
                night_icon    = other.night_icon;
        }
        return *this;
}

WeatherCondition::WeatherCondition(WeatherCondition&& other) :
        name(std::move(other.name)), detailed_name(std::move(other.detailed_name)),
        day_icon(std::move(other.day_icon)), night_icon(std::move(other.night_icon)) {}

WeatherCondition& WeatherCondition::operator=(WeatherCondition&& other) {
        if (this != &other) {
                name          = std::move(other.name);
                detailed_name = std::move(other.detailed_name);
                day_icon      = std::move(other.day_icon);
                night_icon    = std::move(other.night_icon);
        }
        return *this;
}

QString WeatherCondition::getData() const {
        std::string temporary = "{" + name + " " + detailed_name + "}";
        return QString::fromStdString(std::move(temporary));
}

HourlyWeatherData::HourlyWeatherData(const WeatherCondition& default_weather) :
        time(0), weather(&default_weather), temperature(-9999.f), temperature_feels_like(-9999.f),
        temperature_min(-9999.f), temperature_max(-9999.f), atmospheric_pressure(-9999),
        rain(-9999.f), humidity(-9999), wind_speed(-9999.f) {}

HourlyWeatherData::HourlyWeatherData(const HourlyWeatherData& other) :
        time(other.time), weather(other.weather), temperature(other.temperature),
        temperature_feels_like(other.temperature_feels_like),
        temperature_min(other.temperature_min), temperature_max(other.temperature_max),
        atmospheric_pressure(other.atmospheric_pressure), rain(other.rain),
        humidity(other.humidity), wind_speed(other.wind_speed) {}

HourlyWeatherData& HourlyWeatherData::operator=(const HourlyWeatherData& other) {
        if (this != &other) {
                time                   = other.time;
                weather                = other.weather;
                temperature            = other.temperature;
                temperature_feels_like = other.temperature_feels_like;
                temperature_min        = other.temperature_min;
                temperature_max        = other.temperature_max;
                atmospheric_pressure   = other.atmospheric_pressure;
                rain                   = other.rain;
                humidity               = other.humidity;
                wind_speed             = other.wind_speed;
        }
        return *this;
}

HourlyWeatherData::HourlyWeatherData(HourlyWeatherData&& other) :
        time(other.time), weather(other.weather), temperature(other.temperature),
        temperature_feels_like(other.temperature_feels_like),
        temperature_min(other.temperature_min), temperature_max(other.temperature_max),
        atmospheric_pressure(other.atmospheric_pressure), rain(other.rain),
        humidity(other.humidity), wind_speed(other.wind_speed) {}

HourlyWeatherData& HourlyWeatherData::operator=(HourlyWeatherData&& other) {
        if (this != &other) {
                time                   = other.time;
                weather                = other.weather;
                temperature            = other.temperature;
                temperature_feels_like = other.temperature_feels_like;
                temperature_min        = other.temperature_min;
                temperature_max        = other.temperature_max;
                atmospheric_pressure   = other.atmospheric_pressure;
                rain                   = other.rain;
                humidity               = other.humidity;
                wind_speed             = other.wind_speed;
        }
        return *this;
}

void HourlyWeatherData::printData() const {
        // TODO Centralized position for default invalid values like the -9999 here
        qInfo() << "Time:" << time << "Weather:" << weather->getData()
                << "Temperature:" << temperature << "Feels like:" << temperature_feels_like
                << "Min:" << temperature_min << "Max:" << temperature_max
                << "Pressure:" << atmospheric_pressure
                << "Rain:" << ((rain != -9999) ? std::to_string(rain) : "N/A")
                << "Humidity:" << humidity << "Wind speed:" << wind_speed;
}

WeatherData::WeatherData(const std::array<HourlyWeatherData, 39> hours) : hours(hours) {}

std::array<HourlyWeatherData, 39>& WeatherData::getHours() {
        return hours;
}

void WeatherData::printData() const {
        for (auto& hour : hours) hour.printData();
}

const std::unordered_map<int, WeatherCondition> WeatherData::weathers{
        {200, WeatherCondition("Thunderstorm", "thunderstorm with light rain", QImage(), QImage())},
        {201, WeatherCondition("Thunderstorm", "thunderstorm with rain", QImage(), QImage())},
        {202, WeatherCondition("Thunderstorm", "thunderstorm with heavy rain", QImage(), QImage())},
        {210, WeatherCondition("Thunderstorm", "light thunderstorm", QImage(), QImage())},
        {211, WeatherCondition("Thunderstorm", "thunderstorm", QImage(), QImage())},
        {212, WeatherCondition("Thunderstorm", "heavy thunderstorm", QImage(), QImage())},
        {221, WeatherCondition("Thunderstorm", "ragged thunderstorm", QImage(), QImage())},
        {230,
         WeatherCondition("Thunderstorm", "thunderstorm with light drizzle", QImage(), QImage())},
        {231, WeatherCondition("Thunderstorm", "thunderstorm with drizzle", QImage(), QImage())},
        {232,
         WeatherCondition("Thunderstorm", "thunderstorm with heavy drizzle", QImage(), QImage())},

        {300, WeatherCondition("Drizzle", "light intensity drizzle", QImage(), QImage())},
        {301, WeatherCondition("Drizzle", "drizzle", QImage(), QImage())},
        {302, WeatherCondition("Drizzle", "heavy intensity drizzle", QImage(), QImage())},
        {310, WeatherCondition("Drizzle", "light intensity drizzle rain", QImage(), QImage())},
        {311, WeatherCondition("Drizzle", "drizzle rain", QImage(), QImage())},
        {312, WeatherCondition("Drizzle", "heavy intensity drizzle rain", QImage(), QImage())},
        {313, WeatherCondition("Drizzle", "shower rain and drizzle", QImage(), QImage())},
        {314, WeatherCondition("Drizzle", "heavy shower rain and drizzle", QImage(), QImage())},
        {321, WeatherCondition("Drizzle", "shower drizzle", QImage(), QImage())},

        {500, WeatherCondition("Rain", "light rain", QImage(), QImage())},
        {501, WeatherCondition("Rain", "moderate rain", QImage(), QImage())},
        {502, WeatherCondition("Rain", "heavy intensity rain", QImage(), QImage())},
        {503, WeatherCondition("Rain", "very heavy rain", QImage(), QImage())},
        {504, WeatherCondition("Rain", "extreme rain", QImage(), QImage())},
        {511, WeatherCondition("Rain", "freezing rain", QImage(), QImage())},
        {520, WeatherCondition("Rain", "light intensity shower rain", QImage(), QImage())},
        {521, WeatherCondition("Rain", "shower rain", QImage(), QImage())},
        {522, WeatherCondition("Rain", "heavy intensity shower rain", QImage(), QImage())},
        {531, WeatherCondition("Rain", "ragged shower rain", QImage(), QImage())},

        {600, WeatherCondition("Snow", "light snow", QImage(), QImage())},
        {601, WeatherCondition("Snow", "snow", QImage(), QImage())},
        {602, WeatherCondition("Snow", "heavy snow", QImage(), QImage())},
        {611, WeatherCondition("Snow", "sleet", QImage(), QImage())},
        {612, WeatherCondition("Snow", "light shower sleet", QImage(), QImage())},
        {613, WeatherCondition("Snow", "shower sleet", QImage(), QImage())},
        {615, WeatherCondition("Snow", "light rain and snow", QImage(), QImage())},
        {616, WeatherCondition("Snow", "rain and snow", QImage(), QImage())},
        {620, WeatherCondition("Snow", "light shower snow", QImage(), QImage())},
        {621, WeatherCondition("Snow", "shower snow", QImage(), QImage())},
        {622, WeatherCondition("Snow", "heavy shower snow", QImage(), QImage())},

        {701, WeatherCondition("Mist", "mist", QImage(), QImage())},
        {711, WeatherCondition("Smoke", "smoke", QImage(), QImage())},
        {721, WeatherCondition("Haze", "haze", QImage(), QImage())},
        {731, WeatherCondition("Dust", "sand/dust whirls", QImage(), QImage())},
        {741, WeatherCondition("Fog", "fog", QImage(), QImage())},
        {751, WeatherCondition("Sand", "sand", QImage(), QImage())},
        {761, WeatherCondition("Dust", "dust", QImage(), QImage())},
        {762, WeatherCondition("Ash", "volcanic ash", QImage(), QImage())},
        {771, WeatherCondition("Squall", "squalls", QImage(), QImage())},
        {781, WeatherCondition("Tornado", "tornado", QImage(), QImage())},

        {800, WeatherCondition("Clear", "clear sky", QImage(), QImage())},

        {801, WeatherCondition("Clouds", "few clouds: 11-25%", QImage(), QImage())},
        {802, WeatherCondition("Clouds", "scattered clouds: 25-50%", QImage(), QImage())},
        {803, WeatherCondition("Clouds", "broken clouds: 51-84%", QImage(), QImage())},
        {804, WeatherCondition("Clouds", "overcast clouds: 85-100%", QImage(), QImage())},
        {9999, WeatherCondition("NULL", "NULL", QImage(), QImage())}};
