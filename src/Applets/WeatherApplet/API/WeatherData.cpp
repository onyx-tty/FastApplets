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

QString WeatherCondition::getWeatherConditionInfo() const {
        qDebug() << "Name:" << name << "Detailed name:" << detailed_name;
        std::string temporary = "{" + name + " " + detailed_name + "}";
        return QString::fromStdString(std::move(temporary));
}

HourlyWeatherData::HourlyWeatherData(decltype(time) time, decltype(weather) weather,
                                     decltype(temperature)          temperature,
                                     decltype(temperature_min)      temperature_min,
                                     decltype(temperature_max)      temperature_max,
                                     decltype(atmospheric_pressure) atmospheric_pressure,
                                     decltype(rain) rain, decltype(humidity) humidity,
                                     decltype(wind_speed) wind_speed) :
        time(time), weather(weather), temperature(temperature), temperature_min(temperature_min),
        temperature_max(temperature_max), atmospheric_pressure(atmospheric_pressure), rain(rain),
        humidity(humidity), wind_speed(wind_speed) {}

HourlyWeatherData::HourlyWeatherData(const HourlyWeatherData& other) :
        time(other.time), weather(other.weather), temperature(other.temperature),
        temperature_min(other.temperature_min), temperature_max(other.temperature_max),
        atmospheric_pressure(other.atmospheric_pressure), rain(other.rain),
        humidity(other.humidity), wind_speed(other.wind_speed) {}

HourlyWeatherData& HourlyWeatherData::operator=(const HourlyWeatherData& other) {
        if (this != &other) {
                time                 = other.time;
                weather              = other.weather;
                temperature          = other.temperature;
                temperature_min      = other.temperature_min;
                temperature_max      = other.temperature_max;
                atmospheric_pressure = other.atmospheric_pressure;
                rain                 = other.rain;
                humidity             = other.humidity;
                wind_speed           = other.wind_speed;
        }
        return *this;
}

HourlyWeatherData::HourlyWeatherData(HourlyWeatherData&& other) :
        time(other.time), weather(other.weather), temperature(other.temperature),
        temperature_min(other.temperature_min), temperature_max(other.temperature_max),
        atmospheric_pressure(other.atmospheric_pressure), rain(other.rain),
        humidity(other.humidity), wind_speed(other.wind_speed) {}

HourlyWeatherData& HourlyWeatherData::operator=(HourlyWeatherData&& other) {
        if (this != &other) {
                time                 = other.time;
                weather              = other.weather;
                temperature          = other.temperature;
                temperature_min      = other.temperature_min;
                temperature_max      = other.temperature_max;
                atmospheric_pressure = other.atmospheric_pressure;
                rain                 = other.rain;
                humidity             = other.humidity;
                wind_speed           = other.wind_speed;
        }
        return *this;
}

void HourlyWeatherData::setWeatherData(decltype(time) time, decltype(weather) weather,
                                       decltype(temperature)          temperature,
                                       decltype(temperature_min)      temperature_min,
                                       decltype(temperature_max)      temperature_max,
                                       decltype(atmospheric_pressure) atmospheric_pressure,
                                       decltype(rain) rain, decltype(humidity) humidity,
                                       decltype(wind_speed) wind_speed) {
        this->time                 = time;
        this->weather              = weather;
        this->temperature          = temperature;
        this->temperature_min      = temperature_min;
        this->temperature_max      = temperature_max;
        this->atmospheric_pressure = atmospheric_pressure;
        this->rain                 = rain;
        this->humidity             = humidity;
        this->wind_speed           = wind_speed;
}

void HourlyWeatherData::printHourlyWeatherInfo() const {
        qInfo() << "Time:" << time << "Weather:" /*<< weather.getWeatherConditionInfo() */
                << "Temperature:" << temperature << "Min:" << temperature_min
                << "Max:" << temperature_max << "Pressure:" << atmospheric_pressure
                << "Rain:" << rain << "Humidity:" << humidity << "Wind speed:" << wind_speed;
}

void DailyWeatherData::setTemperatureRange() {
        if (hours.empty()) return;

        auto* min = &hours[0].temperature_min;
        auto* max = &hours[0].temperature_max;

        for (int i = 1; i < hours.size(); ++i) {
                if (*min > hours[i].temperature_min) min = &hours[i].temperature_min;
                if (*max < hours[i].temperature_max) max = &hours[i].temperature_max;
        }

        min_temperature = *min;
        max_temperature = *max;
}

DailyWeatherData::DailyWeatherData(std::array<HourlyWeatherData, 8> hours) : hours(hours) {}

void DailyWeatherData::printDailyWeatherInfo() const {
        // TODO What day is it even!?
        qInfo() << "Daily data: " << "min_temperature:" << min_temperature
                << "max_temperature:" << max_temperature;
        for (const auto& hour : hours) { hour.printHourlyWeatherInfo(); }
        qInfo() << "\n";
}

const std::unordered_map<int, const WeatherCondition> HourlyWeatherData::weathers{
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
        {804, WeatherCondition("Clouds", "overcast clouds: 85-100%", QImage(), QImage())}};
