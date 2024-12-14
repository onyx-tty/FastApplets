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

#include "OpenWeather.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <cstdlib>
#include <curl/curl.h>
#include <fstream>
#include <regex>
#include <string>
#include <utility>

// TODO Runtime input of API keys
std::string OpenWeatherAPI::userURL(std::string& api_key) {
        // TODO New layout file
        std::string url = "http://api.openweathermap.org/data/2.5/forecast?id=524901&appid="
                        + api_key;
        return url;
}

std::string OpenWeatherAPI::fetchAPIKey() {
        std::ifstream                       file(dotenv_filepath, std::ifstream::in);
        // Expect to find a 32-character long alphanumeric value followed by '='
        std::pair<std::string, std::string> item{"^OPENWEATHER_API_KEY\\s*=\\s*", "\\w{32}"};

        qInfo() << "Attempting to start the loop in" << __func__;
        for (std::string line; std::getline(file, line);) {
                std::smatch results;
                qInfo() << "Line:" << line;

                if (std::regex_search(line, results, std::regex(item.second))) {
                        item.second = results.str(0);
                        qInfo() << "Found: " << item.second;
                        file.close();
                        return item.second;
                }
        }

        // If no match is found
        qCritical() << "Critical! API key not found!";
        QApplication::quit();
        return "";
}

void OpenWeatherAPI::parseResponse() {
        api_response = json::parse(curl.getResponseAddress());
}

OpenWeatherAPI::OpenWeatherAPI(QWidget* parent, QApplication* app) :
        parent(parent), app(app),
        dotenv_filepath(locateProjectRoot().toStdString() + "/src/Config/.env"),
        api_key(fetchAPIKey()), curl(userURL(api_key)) {
        printResponse();
}

OpenWeatherAPI::~OpenWeatherAPI() {}

void OpenWeatherAPI::printResponse() const {
        qInfo() << api_response.dump(8);
}

const QApplication* OpenWeatherAPI::getApp() const {
        return app;
}

const CurlHandler& OpenWeatherAPI::getCurl() const {
        return curl;
}
