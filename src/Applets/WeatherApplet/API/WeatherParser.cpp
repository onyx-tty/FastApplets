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

#include "WeatherParser.h"
#include "../../../Config/WeatherLayout.h"
#include "../../../Utils/Time.h"
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

WeatherParser::WeatherParser(QWidget* parent, const QApplication& app) :
        open_weather(parent, app) {}

void WeatherParser::updateWeatherData() {
        // fetch data from OpenWeather's API call
        open_weather.callAPI();
        const json& response = open_weather.getResponse();

        // extract and assign weather data from our fetched response
        int index = 0;
        traverseJson(
                "", response, "",
                [this](const std::string& key, const json& data, std::string path, int index) {
                        processWeatherItem(key, data, key, index);
                },
                index);

        // time units in seconds
        constexpr time_t hour = 60 * 60, day = hour * 24;

        // time
        const auto   iter_begin = WeatherData::hours.cbegin(), iter_end = WeatherData::hours.cend();
        const int    hour_spacing     = findHourSpacing((iter_begin + 1)->time, iter_begin->time);
        const time_t current_midnight = findMidnight(), next_midnight = current_midnight + day;

        // debug
        WeatherData::printData();

        // number of blocs for each timestamp, index-friendly
        const int blocs_per_day =
                findWeatherBlocsFitCount(next_midnight, current_midnight, hour_spacing).value();
        const auto first_day_blocs = findWeatherBlocsFitCount(next_midnight, iter_begin->time,
                                                              hour_spacing);
        if (first_day_blocs.value_or(0) > blocs_per_day) {
                qFatal("First day blocs %i is higher than blocs per day %i! Not allowed!",
                       first_day_blocs.value(), blocs_per_day);
        }

        // identify day names of each hour, assigned to hour->day
        // TODO This design may not be perfect, HourlyWeatherData should be able to handle it on its own
        WeatherData::fillDayNames(blocs_per_day, first_day_blocs);

        // print daily weather info for debug purposes
        qDebug() << "Daily weather info from" << __func__ << ":";
        for (const auto& hour : WeatherData::hours) hour.printData();
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
                        qDebug() << "Trying to convert" << input_string << "to int...";
                        output_number = std::stoi(input_string);
                        if (!std::is_integral<decltype(output_number)>::value) {
                                throw std::invalid_argument(
                                        "Expected integral type, received something else\n");
                        }
                } catch (...) { return false; }
                return true;
        };

        // if current key isn't a repeat of last key
        // (and it'll always be if we're traversing an array)
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
                        if (index == WeatherData::hours.size() - 1) { // if this is the last item
                                traverseJson(prime_key, item, path, handler, index);
                                break; // only look for as many hours as we need
                        } else traverseJson(prime_key, item, path, handler, index); // if not

                        // if another item in hour list, we're proceeding to the next hour
                        if (prime_key == "list") { index++; }
                }
        } else {
                handler(prime_key, prime_value, path, index);
        }
}

// TODO This could be refactored with "chunk.at()" in mind
void WeatherParser::processWeatherItem(const std::string& key, const json& value,
                                       const std::string& path, int& index) {
        qDebug() << "Parsing object" << key << ":" << value.dump() << "! Index is" << index;

        if (index < 0 || index >= WeatherData::hours.size()) {
                qFatal("WeatherData index %i out of range in %s, size is only %zu, quitting!",
                       index, __func__, WeatherData::hours.size());
        }

        if (key == "dt") {
                qDebug() << "Found UNIX time:" << value.template get<int>();
                WeatherData::hours[index].time = value.template get<int>();
        } else if (key == "temp") {
                qDebug() << "Found temperature:" << value.template get<float>();
                WeatherData::hours[index].temperature = value;
        } else if (key == "feels_like") {
                qDebug() << "Found temperature_feels_like:" << value.template get<float>();
                WeatherData::hours[index].temperature_feels_like = value;
        } else if (key == "temp_min") {
                qDebug() << "Found temperature_min:" << value.template get<float>();
                WeatherData::hours[index].temperature_min = value;
        } else if (key == "temp_max") {
                qDebug() << "Found temperature_max:" << value.template get<float>();
                WeatherData::hours[index].temperature_max = value;
        } else if (key == "pressure") {
                qDebug() << "Found atmospheric pressure:" << value.template get<int>();
                WeatherData::hours[index].atmospheric_pressure = value;
                // needs to be tested
        } else if (key == "3h") {
                qDebug() << "Found rainfall:" << value.template get<float>();
                WeatherData::hours[index].rain = value;
        } else if (key == "humidity") {
                qDebug() << "Found humidity:" << value.template get<float>();
                WeatherData::hours[index].humidity = value;
        } else if (key == "speed") {
                qDebug() << "Found wind speed:" << value.template get<float>();
                WeatherData::hours[index].wind_speed = value;
        } else if (key == "id") {
                // TODO WeatherCondition reference instead of a separate weather_id int
                const int weather_id = value.template get<int>();
                if (WeatherLayoutManager::layout_prop.weather_list.find(weather_id)
                    != WeatherLayoutManager::layout_prop.weather_list.end()) {
                        WeatherData::hours[index].weather = &WeatherLayoutManager::layout_prop
                                                                     .weather_list.at(weather_id);
                        qDebug() << "And its weather_id is" << weather_id << "corresponding to"
                                 << WeatherLayoutManager::layout_prop.weather_list.at(weather_id)
                                            .getData();
                }
        } else {
                qDebug() << "Parsed irrelevant key:" << key;
        }
}
