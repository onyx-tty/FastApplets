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

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QString>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

using json = nlohmann::json;

std::array<HourlyWeatherData, 8> debugInitHours() {
        qInfo() << "Starting" << __func__;
        static auto weather = WeatherCondition("blank", "blank", QImage(), QImage());
        static auto hour    = HourlyWeatherData(0, *&weather, 0, 0, 0, 0, 0, 0, 0);

        return std::array<HourlyWeatherData, 8>{*&hour, *&hour, *&hour, *&hour,
                                                *&hour, *&hour, *&hour, *&hour};
}

WeatherParser::WeatherParser(QWidget* parent, const QApplication* app,
                             const WeatherEnvProp& env_prop) :
        open_weather(parent, app, env_prop), weather_data(debugInitHours()) {}

void WeatherParser::updateWeatherData() {
        qInfo() << "Starting" << __func__;
        using std::pair;
        open_weather.callAPI();
        const json& response = open_weather.getResponse();

        qInfo() << "Starting recursion in" << __func__;
        recursiveJsonIteration(pair(std::string(""), response), [this](const json& response, int i) {
                extractHourlyWeather(response, i);
        });

        qInfo() << "Setting temperature range in" << __func__;
        weather_data.setTemperatureRange();
        qInfo() << "Printing daily weather info in" << __func__;
        weather_data.printDailyWeatherInfo();

        qInfo() << "SUCCESS!";
        QApplication::quit(); // TODO For now, all we need is info display!
}

// TODO Too nested, refactor and optimize
void WeatherParser::recursiveJsonIteration(const std::pair<const std::string&, const json&> response,
                                           const std::function<void(const json&, int)> parseItem) {
        using std::pair;
        static int index_buffer;

        qInfo() << "---------------------------------------------------------------------";
        qInfo() << "Starting recursion in " << __func__ << "!"
                << "index_buffer = " << index_buffer;

        if (response.second.is_object()) {
                qInfo() << "DESCEND! OBJECT";
                for (auto& [key, value] : response.second.items()) {
                        qInfo() << key;
                        if (value.is_array() || value.is_object()) {
                                qInfo() << key << ":" << value.dump().substr(0, 100)
                                        << "\nNew layer of recursion will now proceed for"
                                        << value.dump().substr(0, 100) << "!";
                                recursiveJsonIteration(pair(key, value), parseItem);
                        }
                }
        } else if (response.second.is_array()) {
                qInfo() << "DESCEND! ARRAY";
                qInfo() << "Given response is an array! Starting a for loop";
                for (auto& [key, value] : response.second.items()) {
                        qInfo() << key;
                        if (key == "list") {
                                qInfo() << "Found list of hours, a for loop will now start!";
                                for (int i = 0; i < value.size(); ++i) {
                                        index_buffer = i;
                                        qInfo() << "index_buffer updated to match the new hour, "
                                                << index_buffer
                                                << ", new layer of recursion will now proceed!";
                                        recursiveJsonIteration(pair(key, value[i]), parseItem);
                                }
                        } else if (key == "main") {
                                qInfo() << "Found temperature values for index " << index_buffer;
                                for (auto& [key2, value2] : value.items()) {
                                        qInfo() << "Parsing " << value.template get<std::string>();
                                        parseItem(value2, index_buffer);
                                }
                        } else if (key == "weather") {
                                qInfo() << "Key is a weather nest, fetching the weather ID within " "it, parsing:"
                                        << value.template get<std::string>();
                                parseItem(value[index_buffer].at("id"), index_buffer);
                        } else {
                                qInfo() << "We haven't received a recognizable key, our key is:"
                                        << key;
                                for (auto& item : value) {
                                        qInfo() << key << ":" << item.dump().substr(0, 100)
                                                << "\nNew recursive layer will now begin!";
                                        recursiveJsonIteration(pair(key, item), parseItem);
                                }
                        }
                }
        } else if (response.second.is_string() || response.second.is_number()
                   || response.second.is_boolean()) {
                qInfo() << "PARSE! VARIABLE";
                qInfo() << "Given variable" << response.second.dump() << "belonging to"
                        << response.first;
                parseItem(response.second, index_buffer);
        } else {
                qInfo() << "FAIL! Unpredictable & uniterable?";
                qWarning() << "Given flawed object" << response.second.type_name() << response.first
                           << QString("in " + QString(__func__))
                           << ", this could be a result of failed control flow!";
        }

        return;
}

// TODO This could be refactored with "chunk.at()" in mind
void WeatherParser::extractHourlyWeather(const json& chunk, const int i) {
        qInfo() << "Parsing object " << chunk.dump() << "!";
        for (auto& [key, value] : chunk.items()) {
                if (key == "dt") {
                        qInfo() << "Found current date in UNIX time: " << value.template get<int>();
                        weather_data.hours[i].time = value.template get<int>();
                } else if (key == "temp") {
                        qInfo() << "Found temperature: " << value.template get<float>();
                        weather_data.hours[i].temperature = value;
                } else if (key == "pressure") {
                        qInfo() << "Found atmospheric pressure: " << value.template get<int>();
                        weather_data.hours[i].atmospheric_pressure = value;
                } else if (key == "rain") {
                        qInfo() << "Found rainfall: " << value.template get<float>();
                        weather_data.hours[i].rain = value;
                } else if (key == "humidity") {
                        qInfo() << "Found humidity: " << value.template get<float>();
                        weather_data.hours[i].humidity = value;
                } else if (key == "wind") {
                        qInfo() << "Found wind: " << value.template get<int>();
                        weather_data.hours[i].wind_speed = value;
                } else if (key == "id") {
                        auto weather_address = weather_data.hours[i].weathers.find(
                                value.template get<unsigned>());
                        if (weather_address != weather_data.hours[i].weathers.end()) {
                                auto& weather = weather_address->second;
                                qInfo() << "Found weather id: " << value.template get<int>()
                                        << " which corresponds to: " << weather.name << " "
                                        << weather.detailed_name;
                                weather_data.hours[i].weather = weather_data.hours[i].weathers.at(
                                        value);
                        }
                } else {
                        qDebug() << "Unexpected key: " << key;
                }
        }
}

const int WeatherParser::findClosestHour() const {
        using namespace std::chrono;
        const auto now_unprocessed = system_clock::to_time_t(system_clock::now());
        const auto now             = *localtime(&now_unprocessed);
        for (int first = 0; first + 3 <= 24; first = first + 3) {
                int second = first + 3;
                if (first <= now.tm_hour && now.tm_hour < second) {
                        // We convert hours to minutes before performing an operation
                        int current_minute = (now.tm_hour * 60) + now.tm_min;
                        int diff1          = std::abs(current_minute - (first * 60));
                        int diff2          = std::abs(current_minute - (second * 60));
                        return std::min(diff1, diff2);
                } else continue;
        }
        qFatal("Something went wrong because %s wasn't able to find the closest hour!", __func__);
};
