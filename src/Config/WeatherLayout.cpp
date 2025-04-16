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

#include "WeatherLayout.h"

#include <QDebug>
#include <QImage>

#include <fstream>
#include <regex>

using global = LayoutManager;

WeatherMainWindowProp::WeatherMainWindowProp() :
        size(global::main_window_prop.size), title(global::main_window_prop.title) {};

WeatherEnvProp::WeatherEnvProp(const QApplication* app) : EnvProp(app) {};

const WeatherEnvProp& WeatherLayoutManager::getEnvProp(const QApplication* app) {
        return static_cast<WeatherEnvProp&>(LayoutManager::getEnvProp(app));
}

std::string& WeatherEnvProp::getOpenWeatherKey() const { // TODO Too nested, clean this up
        // Expect to find a 32-character long alphanumeric key followed by '='
        static std::pair<std::string, std::string> item{"^OPENWEATHER_API_KEY\\s*=\\s*", "\\w{32}"};
        static bool                                api_key_initialized = false;

        if (!api_key_initialized) {
                std::ifstream file(dotenv_filepath.toStdString(), std::ifstream::in);
                qInfo() << "Attempting to start the loop in" << __func__;
                for (std::string line; std::getline(file, line);) {
                        std::smatch results;
                        qInfo() << "Line:" << line;

                        // if we find our API key, trigger this
                        if (std::regex_search(line, results, std::regex(item.second))) {
                                item.second = results.str(0);
                                qInfo() << "Found: " << item.second;
                                file.close();
                                api_key_initialized = true;
                                return item.second;
                        }
                }
                // else if API key wasn't found in the previous loop
                qCritical() << "Critical! API key not found!";
                QApplication::quit();
        }

        return item.second;
}

WeatherStyleProp::WeatherStyleProp() :
        selected(global::style_prop.selected), unselected(global::style_prop.unselected) {};

WeatherLayoutProp::WeatherLayoutProp() {}

const std::unordered_map<int, WeatherCondition> WeatherLayoutProp::weather_list{
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

/*
const std::array<WeatherCellGrid*, 2> WeatherLayoutProp::initCellGrid(QWidget*     parent,
                                                                      QHBoxLayout* layout,
                                                                      WeatherData& weather_data) {
        using std::array;
        using std::make_tuple;
        using std::to_string;
        using std::vector;

        if (!parent || !layout) {
                qFatal() << "Invalid parent and/or layout in" << __func__ << "!\n";
                QApplication::quit();
        }

        auto& hours = weather_data.hours;
        auto* now   = hours.begin();

        // TODO Unsafe and inflexible, requires a rework
        WeatherCellGrid* today_cells = new WeatherCellGrid{
                {"current_weather", {QPixmap(), {now->weather->name, now->weather->detailed_name}}},
                {"current_temperature",
                 {QPixmap(),
                  {"Temperature", QString::number(now->temperature), "Feels like",
                   QString::number(now->temperature_feels_like)}}},
                {"city_name", {QPixmap(), {}
}
}
, {"current_wind_speed", {QPixmap(), {"Wind", QString::number(now->wind_speed)}}},
        {"current_humidity", {QPixmap(), {"Humidity", QString::number(now->humidity)}}},
        {"current_rainfall", {QPixmap(), {"Rain", QString::number(now->rain)}}}, {
        "current_pressure", {
                QPixmap(), {
                        "Pressure", QString::number(now->atmospheric_pressure)
                }
        }
}
}
;

// TODO Calculate daily min-max temperatures, the current state is just a placeholder
auto weekDay = [](decltype(now) hour, QString day_name) -> vector<QString> {
        return vector<QString>{std::move(day_name), QString::number(hour->temperature_max),
                               QString::number(hour->temperature_min)};
};

// TODO Find day of the week from the UNIX timestamp
constexpr int    hours_in_day = 8;
WeatherCellGrid* weekly_cells =
        new WeatherCellGrid{{"day1", {QPixmap(), weekDay((now + (hours_in_day * 0)), "Today")}},
                            {"day2", {QPixmap(), weekDay((now + (hours_in_day * 1)), "Tomorrow")}},
                            {"day3", {QPixmap(), weekDay((now + (hours_in_day * 2)), "Day 3")}},
                            {"day4", {QPixmap(), weekDay((now + (hours_in_day * 3)), "Day 4")}},
                            {"day5", {QPixmap(), weekDay((now + (hours_in_day * 4)), "Day 5")}}};

        WeatherCellGrid* detail_cells = new WeatherCellGrid{

        };

return {std::move(today_cells), std::move(weekly_cells)};
}

const std::array<WeatherCellGrid*, 2>& WeatherLayoutProp::getCellGrid() {
        // If any of the weather cells is nullptr, crash
        if (std::any_of(weather_cell_grid.cbegin(), weather_cell_grid.cend(),
                        [](WeatherCellGrid* cell) -> bool { return !cell; })) {
                qFatal() << "Weather cell grid not initialized properly in" << __func__ << "!\n";
                QApplication::quit();
        }

        return weather_cell_grid;
}
*/

WeatherLayoutManager::WeatherLayoutManager() {};

WeatherMainWindowProp WeatherLayoutManager::main_window_prop;
WeatherStyleProp      WeatherLayoutManager::style_prop;
WeatherLayoutProp     WeatherLayoutManager::layout_prop;
