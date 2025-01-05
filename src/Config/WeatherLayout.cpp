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

std::string& WeatherEnvProp::getOpenWeatherKey(
        const QString& marker) { // TODO Too nested, clean this up
        // Expect to find a 32-character long alphanumeric key followed by '='
        static std::pair<std::string, std::string> item{"^OPENWEATHER_API_KEY\\s*=\\s*", "\\w{32}"};
        static bool                                api_key_initialized = false;
        if (dotenv_filepath.empty()) { initDotenvFilepath(*app, marker); }

        if (!api_key_initialized) {
                std::ifstream file(dotenv_filepath, std::ifstream::in);
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

WeatherEnvProp::WeatherEnvProp(QApplication* app) : EnvProp(app) {};

WeatherEnvProp& WeatherLayoutManager::getEnvProp(QApplication* app) {
        return static_cast<WeatherEnvProp&>(LayoutManager::getEnvProp(app));
}

WeatherLayoutManager::WeatherLayoutManager() {};
WeatherLayoutManager::WeatherLayoutManager(QApplication* app) {};
