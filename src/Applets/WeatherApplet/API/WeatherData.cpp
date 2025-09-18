/* Pretty Applets

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

#include "WeatherData.h"
#include "../../../Config/WeatherLayout.h"

#include <QDebug>

#include <algorithm>

// prevent external linkage via static
static std::array<HourlyWeatherData, HOURLY_WEATHER_DATA_HOURS> debugInitHours() {
        const auto hour = HourlyWeatherData();

        // TODO I can already tell this isn't a good idea
        return std::array<HourlyWeatherData, HOURLY_WEATHER_DATA_HOURS>{
                *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour, *&hour};
}

WeatherCondition::WeatherCondition(QString name, QString detailed_name, const QImage& day_icon,
                                   const QImage& night_icon) :
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
        return QString("{" + name + " : " + detailed_name + "}");
}

HourlyWeatherData::HourlyWeatherData() :
        day("N/A"), time(0), city_name("N/A"), weather(&WeatherLayoutProp::weather_list.at(9999)),
        temperature(-9999.f), temperature_feels_like(-9999.f), temperature_min(-9999.f),
        temperature_max(-9999.f), atmospheric_pressure(-9999), rain(-9999.f), humidity(-9999),
        wind_speed(-9999.f) {}

HourlyWeatherData::HourlyWeatherData(const HourlyWeatherData& other) :
        day(other.day), time(other.time), city_name(other.city_name), weather(other.weather),
        temperature(other.temperature), temperature_feels_like(other.temperature_feels_like),
        temperature_min(other.temperature_min), temperature_max(other.temperature_max),
        atmospheric_pressure(other.atmospheric_pressure), rain(other.rain),
        humidity(other.humidity), wind_speed(other.wind_speed) {}

HourlyWeatherData& HourlyWeatherData::operator=(const HourlyWeatherData& other) {
        if (this != &other) {
                day                    = other.day;
                time                   = other.time;
                city_name              = other.city_name;
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
        day(other.day), time(other.time), city_name(other.city_name), weather(other.weather),
        temperature(other.temperature), temperature_feels_like(other.temperature_feels_like),
        temperature_min(other.temperature_min), temperature_max(other.temperature_max),
        atmospheric_pressure(other.atmospheric_pressure), rain(other.rain),
        humidity(other.humidity), wind_speed(other.wind_speed) {}

HourlyWeatherData& HourlyWeatherData::operator=(HourlyWeatherData&& other) {
        if (this != &other) {
                day                    = std::move(other.day);
                time                   = other.time;
                city_name              = std::move(other.city_name);
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
        // TODO std::optional
        qInfo() << "Day:" << day << "Time:" << time << "City name:" << city_name
                << "Weather:" << weather->getData() << "Temperature:" << temperature
                << "Feels like:" << temperature_feels_like << "Min:" << temperature_min
                << "Max:" << temperature_max << "Pressure:" << atmospheric_pressure
                << "Rain:" << ((rain != -9999) ? QString::number(rain) : "N/A")
                << "Humidity:" << humidity << "Wind speed:" << wind_speed;
}

std::array<HourlyWeatherData, HOURLY_WEATHER_DATA_HOURS> WeatherData::hours(debugInitHours());

void WeatherData::printData() {
        for (auto& hour : hours) hour.printData();
}

void WeatherData::fillDayNames(const int&                      blocs_per_day,
                               const std::optional<const int>& first_day_blocs) {
        if (first_day_blocs.value_or(0) > blocs_per_day) {
                qFatal("First day blocs %i is higher than blocs per day %i! Not allowed! WD",
                       first_day_blocs.value(), blocs_per_day);
        }
        const int  last_day_blocs = blocs_per_day - first_day_blocs.value_or(0);
        const auto begin = hours.begin(), end = hours.end();
        std::pair<HourlyWeatherData*, HourlyWeatherData*>
                      bloc_border{begin, (begin + first_day_blocs.value_or(0))};
        const QString error_message = "In" + QString(__func__)
                                    + "current_day iterator has been reached prematurely"
                                    + "This implies faulty code in that function, repairs needed!";
        const std::array<QString, 6> day{"Today", "Tomorrow", "Day 3", "Day 4", "Day 5", "Day 6"};
        auto                         current_day = day.cbegin();
        const auto                   setDayName  = [&current_day](HourlyWeatherData* hour) {
                static int cnt = 0;
                if (!hour) {
                        qFatal() << "HourlyWeatherData object given in" << __func__
                                 << "is null! Quitting...";
                } else qDebug() << hour->time;
                if (!current_day) {
                        qFatal() << "current_day no." << cnt << "in" << __func__
                                 << "out of scope! Quitting...";
                } else qDebug() << "Current day no." << cnt << "is" << *current_day;
                hour->day = *current_day;
                cnt++;
        };

        // First day
        if (current_day != day.end()) {
                qDebug() << "First for_each group call of setDayName will now proceed!";
                std::for_each(&bloc_border.first, &bloc_border.second, setDayName);
                current_day++;
                bloc_border.first  += first_day_blocs.value_or(0);
                bloc_border.second += blocs_per_day;
        }
        // Days in-between
        for (; bloc_border.second + blocs_per_day < end - last_day_blocs;
             bloc_border.first += blocs_per_day, bloc_border.second += blocs_per_day) {
                if (current_day < day.end()) {
                        qDebug() << "Second for_each group call of setDayName will now proceed!";
                        std::for_each(&bloc_border.first, &bloc_border.second, setDayName);
                } else {
                        qFatal("%s", error_message.toStdString().c_str());
                }
                current_day++;
        }
        // Last day
        bloc_border.first  = (last_day_blocs == 0) ? end : end - last_day_blocs;
        bloc_border.second = end;
        if (bloc_border.first != end) {
                qDebug() << "Third for_each group call of setDayName will now proceed!";
                std::for_each(&bloc_border.first, &bloc_border.second, setDayName);
                current_day++;
        } else {
                qFatal("%s", error_message.toStdString().c_str());
        }
}
