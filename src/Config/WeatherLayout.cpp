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

#include "WeatherLayout.h"

#include <QDebug>
#include <QImage>

#include <fstream>
#include <regex>

using global = LayoutManager;

/* These are settings exclusive to the WeatherApplet. Modify below to adjust application style. */
/* To overwrite defaults, replace parts preceded by the global namespace. */
/* For modified defaults, refer to DefaultLayout.cpp */

/* LayoutManager */
const WeatherMainWindowProp WeatherLayoutManager::main_window_prop(global::main_window_prop.size,
                                                                   global::main_window_prop.title);
const WeatherStyleProp      WeatherLayoutManager::style_prop(global::style_prop.button_stylesheet);
const WeatherLayoutProp     WeatherLayoutManager::layout_prop(global::layout_prop.button_policy,
                                                              QSizePolicy(QSizePolicy::Expanding,
                                                                          QSizePolicy::Expanding));

/* WeatherMainWindowProp */
WeatherMainWindowProp::WeatherMainWindowProp(const QSize size, const QString title) :
        MainWindowProp(size, title), size(size), title(title) {};
/* WeatherStyleProp */
WeatherStyleProp::WeatherStyleProp(const QString button_stylesheet) :
        StyleProp(button_stylesheet), button_stylesheet(button_stylesheet) {};

/* WeatherLayoutProp */
WeatherLayoutProp::WeatherLayoutProp(const QSizePolicy button_policy,
                                     const QSizePolicy cell_policy) :
        LayoutProp(button_policy), cell_policy(cell_policy) {};

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

/* WeatherEnvProp */
WeatherEnvProp::WeatherEnvProp() {};

const WeatherEnvProp& WeatherLayoutManager::getEnvProp() {
        return env_prop;
}

// TODO Too nested, clean this up
const std::string& WeatherEnvProp::getOpenWeatherKey(const QApplication& app) {
        // Expect to find a 32-character long alphanumeric key after '='
        static std::pair<std::string, std::string> item{"^OPENWEATHER_API_KEY\\s*=\\s*", "\\w{32}"};
        static bool                                api_key_initialized = false;

        if (!api_key_initialized) {
                std::ifstream file(global::getEnvProp().getDotenvFilepath().toStdString(),
                                   std::ifstream::in);
                for (std::string line; std::getline(file, line);) {
                        std::smatch results;
                        qDebug() << "Line:" << line;

                        // if we find our API key, trigger this
                        if (std::regex_search(line, results, std::regex(item.second))) {
                                item.second = results.str(0);
                                qDebug() << "OpenWeather API key found!";
                                file.close();
                                api_key_initialized = true;
                                return item.second;
                        }
                }
                // else if API key wasn't found in the previous loop
                qCritical() << "OpenWeather API key not found!";
                QApplication::quit();
        }

        return item.second;
}
