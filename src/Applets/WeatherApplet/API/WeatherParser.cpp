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

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>

using json = nlohmann::json;

std::array<HourlyWeatherData, 39> debugInitHours() {
        // TODO Reusable debug function printing out the invoking function's name
        qDebug() << "Starting" << __func__;
        auto hour = HourlyWeatherData(WeatherData::weathers.at(9999));

        // TODO I can already tell this isn't a good idea
        return std::array<HourlyWeatherData, 39>{*&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                                                 *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                                                 *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                                                 *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                                                 *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                                                 *&hour, *&hour, *&hour, *&hour, *&hour, *&hour,
                                                 *&hour, *&hour, *&hour};
}

WeatherParser::WeatherParser(QWidget* parent, const QApplication* app,
                             const WeatherEnvProp& env_prop) :
        open_weather(parent, app, env_prop), weather_data(debugInitHours()) {}

void WeatherParser::updateWeatherData() {
        qDebug() << "--------------------------";
        qDebug() << "Starting" << __func__ << "!";
        open_weather.callAPI();
        const json& response = open_weather.getResponse();

        qDebug() << "Recursion begins in" << __func__;
        int index = 0;
        traverseJson(
                "", response, "",
                [this](const std::string& key, const json& data, std::string path, int index) {
                        processWeatherItem(key, data, key, index);
                },
                index);

        qDebug() << "Printing daily weather info in" << __func__;
        for (auto& hour : weather_data.getHours()) hour.printData();

        QApplication::quit(); // TODO For now, all we need is info display!
}

// TODO Too nested, refactor and optimize
void WeatherParser::traverseJson(
        const std::string& prime_key, const json& prime_value, std::string path,
        const std::function<void(const std::string&, const json&, std::string, int)>& handler,
        int&                                                                          index) {
        // TODO Move elsewhere
        auto tryStringToIntConversion = [](const std::string& input_string,
                                           int&               output_number) -> bool {
                try {
                        qDebug() << "Trying to convert key" << input_string << "...";
                        output_number = std::stoi(input_string);
                        if (!std::is_integral<decltype(output_number)>::value) {
                                throw std::invalid_argument(
                                        "Expected integral type, received something else\n");
                        }
                } catch (...) {
                        return false;
                }
                return true;
        };

        // if current key isn't a repeat of last key
        // (which is always going to happen if we're traversing an array)
        if (path.empty() || path.size() < prime_key.size()
            || path.substr(path.size() - prime_key.size()) != prime_key)
                path += ("/" + prime_key);
        qDebug() << "Path:" << path << ", index:" << index;

        if (prime_value.is_object()) {
                for (const auto& [key, value] : prime_value.items()) {
                        traverseJson(key, value, path, handler, index);
                }
        } else if (prime_value.is_array()) {
                for (const auto& item : prime_value) {
                        if (index
                            == weather_data.getHours().size() - 1) { // if this is the last item
                                traverseJson(prime_key, item, path, handler, index);
                                break; // only look for as many hours as we need
                        } else traverseJson(prime_key, item, path, handler, index); // if not

                        // if another item in hour list, we're proceeding to the next hour
                        if (prime_key == "list") {
                                index++;
                        }
                }
        } else {
                handler(prime_key, prime_value, path, index);
        }
}

// TODO This could be refactored with "chunk.at()" in mind
void WeatherParser::processWeatherItem(const std::string& key, const json& value,
                                       const std::string& path, int& index) {
        qDebug() << "Parsing object" << key << ":" << value.dump() << "! Index is" << index;

        if (index < 0 || index >= weather_data.getHours().size()) {
                qFatal("Index out of range: %i", index);
                return;
        }

        if (key == "dt") {
                qDebug() << "Found current date in UNIX time:" << value.template get<int>();
                weather_data.getHours()[index].time = value.template get<int>();
        } else if (key == "temp") {
                qDebug() << "Found temperature:" << value.template get<float>();
                weather_data.getHours()[index].temperature = value;
        } else if (key == "temp_min") {
                qDebug() << "Found temperature_min:" << value.template get<float>();
                weather_data.getHours()[index].temperature_min = value;
        } else if (key == "temp_max") {
                qDebug() << "Found temperature_max:" << value.template get<float>();
                weather_data.getHours()[index].temperature_max = value;
        } else if (key == "pressure") {
                qDebug() << "Found atmospheric pressure:" << value.template get<int>();
                weather_data.getHours()[index].atmospheric_pressure = value;
        // TODO Rain is more complicated and can only be obtained if it rains, so for the time being it won't work
        } else if (key == "rain") {
                qDebug() << "Found rainfall:" << value.template get<float>();
                weather_data.getHours()[index].rain = value;
        } else if (key == "humidity") {
                qDebug() << "Found humidity:" << value.template get<float>();
                weather_data.getHours()[index].humidity = value;
        } else if (key == "wind") {
        // TODO Wind is more complicated and has its own nest
                qDebug() << "Found wind:" << value.template get<int>();
                weather_data.getHours()[index].wind_speed = value;
        } else if (key == "id") {
                // TODO WeatherCondition reference instead of separate weather_id int
                int weather_id = value.template get<int>();
                if (weather_data.weathers.find(weather_id) != weather_data.weathers.end()) {
                        weather_data.getHours()[index].weather = &weather_data.weathers.at(
                                weather_id);
                        qDebug() << "And its weather_id is" << weather_id << "corresponding to"
                                 << weather_data.weathers.at(weather_id).getData();
                }
        } else {
                qWarning() << "Unexpected key" << key;
        }
}
