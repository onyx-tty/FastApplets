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

#include <fstream>
#include <regex>

using global = LayoutManager;

WeatherMainWindowProp::WeatherMainWindowProp() :
        size(global::main_window_prop.size), title(global::main_window_prop.title) {};

WeatherStyleProp::WeatherStyleProp() :
        selected(global::style_prop.selected), unselected(global::style_prop.unselected) {};

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

WeatherEnvProp::WeatherEnvProp(const QApplication* app) : EnvProp(app) {};

/*
std::array<WeatherCellGrid*, 3> WeatherLayoutProp::cellGridSingleton(QWidget*     parent,
                                                                     QHBoxLayout* layout,
                                                                     bool&&       is_instantiated) {
        if (!parent || !layout) {
                qFatal() << "Invalid parent and/or layout in" << __func__ << "!\n";
                QApplication::quit();
        }
        if (!is_instantiated) {
                qCritical() << "Singleton not initialized properly in" << __func__ << "!\n";
                QApplication::quit();
        }

        WeatherCellGrid* today_cells =
                new WeatherCellGrid{{QString("current_weather"), {QPixmap(), {}}},
                                    {QString("current_temperature"), {QPixmap(), {}}},
                                    {QString("city_name"), {QPixmap(), {}}},
                                    {QString("current_wind_speed"), {QPixmap(), {}}},
                                    {QString("current_humidity"), {QPixmap(), {}}},
                                    {QString("current_rainfall"), {QPixmap(), {}}},
                                    {QString("current_atmosp")}
                };
        
        WeatherCellGrid* detail_cells = new WeatherCellGrid{

        };
        WeatherCellGrid* weekly_cells = new WeatherCellGrid{

        };

        std::array<WeatherCellGrid*, 3> compound_cells = {today_cells, detail_cells, weekly_cells};

        return compound_cells;
}
*/

const WeatherEnvProp& WeatherLayoutManager::getEnvProp(const QApplication* app) {
        return static_cast<WeatherEnvProp&>(LayoutManager::getEnvProp(app));
}

WeatherLayoutManager::WeatherLayoutManager() {};
WeatherLayoutManager::WeatherLayoutManager(const QApplication* app) {};
