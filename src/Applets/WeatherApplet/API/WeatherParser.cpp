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
#include "../../../../modules/TraverseJSON/include/TraverseJSON.h"
#include "../../../Config/WeatherLayout.h"
#include "../../../Utils/Time.h"
#include "../../../Utils/TimeConstants.h"
#include "WeatherData.h"

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QString>

#include <ctime>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

using callback  = std::function<void(const std::string&, const json&, std::string, int)>;
using predicate = std::function<bool(int)>;

void WeatherParser::updateWeatherData(const QApplication& app) {
        std::string root_key, path;
        // fetch data from OpenWeather's API call
        const json& response = OpenWeatherAPI::fetchWeatherReport(app);
        int         index      = 0;
        // extract and assign each node encountered in our fetched response
        auto handleNode = [](const std::string& key, const json& data, std::string path, int index) {
                processWeatherItem(key, data, key, index);
        };
        // stop traversing the response if we've covered all hours
        auto predicate = [](int index) -> bool { return index == (WeatherData::hours.size() - 1); };

        // traverse the JSON file, handling each node, and ending the loop when the predicate returns false
        TraverseJSON::traverseJSON(root_key, response, path, index, handleNode, predicate);

        // set up iteration range
        const auto   iter_begin = WeatherData::hours.cbegin(), iter_end = WeatherData::hours.cend();
        // figure out the exact hour spacing by analyzing the time difference between two hours
        const int    hour_spacing     = findHourSpacing((iter_begin + 1)->time, iter_begin->time);
        // detect current midnight's and next midnight's UNIX timestamps
        const time_t current_midnight = findMidnight(), next_midnight = current_midnight + epoch_duration::day;

        // number of blocs we'll receive for each day, index-friendly
        // a bloc is a collection of data corresponding to a particular hour
        const int blocs_per_day =
                findWeatherBlocsFitCount(next_midnight, current_midnight, hour_spacing).value();
        // first_day_blocs + last_day_blocs = blocs_per_day
        const auto first_day_blocs = findWeatherBlocsFitCount(next_midnight, iter_begin->time,
                                                              hour_spacing);
        if (first_day_blocs.value_or(0) > blocs_per_day) {
                qFatal("First day blocs %i is higher than blocs per day %i! Not allowed!",
                       first_day_blocs.value(), blocs_per_day);
        }

        // identify day names of each hour, assigned to hour->day
        // TODO This design may not be perfect, HourlyWeatherData should be able to handle it on its own
        WeatherData::fillDayNames(blocs_per_day, first_day_blocs);

        // debug
        // print total weather data
        WeatherData::printData();
        // print daily weather data
        qDebug() << "Daily weather info from" << __func__ << ":";
        for (const auto& hour : WeatherData::hours) hour.printData();
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
