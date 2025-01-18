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

#include "WeatherParser.h"
#include "WeatherData.h"

#include <QDebug>
#include <QString>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

WeatherParser::WeatherParser(QWidget* parent, QApplication* app, WeatherEnvProp& env_prop) :
        open_weather(parent, app, env_prop) {}

void WeatherParser::updateWeatherData() {
        using std::pair;
        open_weather.callAPI();
        const json& response = open_weather.getResponse();

        recursiveJsonIteration(pair(std::string(""), response), [this](const json& response, int i) {
                extractHourlyWeather(response, i);
        });
}

// TODO Too nested, refactor and optimize
void WeatherParser::recursiveJsonIteration(std::pair<const std::string&, const json&> response,
                                           std::function<void(const json&, int)>      parseItem) {
        using std::pair;
        static int index_buffer;

        if (response.second.is_object()) {
                for (auto& [key, value] : response.second.items()) {
                        if (value.is_array() || value.is_object()) {
                                recursiveJsonIteration(pair(key, value), parseItem);
                        }
                }
        } else if (response.second.is_array()) {
                for (auto& [key, value] : response.second.items()) {
                        if (key == "list") {
                                for (int i = 0; i < value.size(); ++i) {
                                        index_buffer = i;
                                        recursiveJsonIteration(pair(key, value[i]), parseItem);
                                }
                        } else if (key == "main") {
                                for (auto& [key2, value2] : value.items())
                                        parseItem(value2, index_buffer);
                        } else if (key == "weather") {
                                parseItem(value[index_buffer].at("id"), index_buffer);
                        }
                }
        } else {
                qWarning() << "Given uniterable object" << response.second.type_name()
                           << response.first << "in" << __func__
                           << ", this could be a result of failed control flow!";
        }

        return;
}

unsigned WeatherParser::findClosestHour() {
        using namespace std::chrono;
        const auto now_unprocessed = system_clock::to_time_t(system_clock::now());
        const auto now             = *localtime(&now_unprocessed);
        for (int first = 0; first + 3 <= 24; first = first + 3) {
                int second = first + 3;
                if (first <= now.tm_hour && now.tm_hour < second) {
                        // We convert hours to minutes before performing an operation
                        int      current_minute = (now.tm_hour * 60) + now.tm_min;
                        unsigned diff1          = std::abs(current_minute - (first * 60));
                        unsigned diff2          = std::abs(current_minute - (second * 60));
                        return std::min(diff1, diff2);
                } else continue;
        }
        qFatal("Something went wrong because %s wasn't able to find the closest hour!", __func__);
};

// TODO This could be refactored with "chunk.at()" in mind
void WeatherParser::extractHourlyWeather(const json& chunk, int i) {
        for (auto& [key, value] : chunk.items()) {
                if (key == "dt") {
                        weather_data.hours[i].time = value.template get<int>();
                } else if (key == "temp") {
                        weather_data.hours[i].temperature = value;
                } else if (key == "pressure") {
                        weather_data.hours[i].atmospheric_pressure = value;
                } else if (key == "rain") {
                        weather_data.hours[i].rain = value;
                } else if (key == "humidity") {
                        weather_data.hours[i].humidity = value;
                } else if (key == "wind") {
                        weather_data.hours[i].wind_speed = value;
                } else if (key == "id") {
                        weather_data.hours[i].weather = weather_data.hours[i].weathers.at(value);
                }
        }
};
