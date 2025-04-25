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

#include "OpenWeather.h"
#include "../../../Config/WeatherLayout.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <cstdlib>
#include <curl/curl.h>
#include <string>

// TODO Runtime input of API keys
const std::string OpenWeatherAPI::getUserURL() {
        // TODO New layout file
        const std::string url = "http://api.openweathermap.org/data/2.5/forecast?id=524901&appid="
                              + WeatherLayoutManager::getEnvProp().getOpenWeatherKey(app);
        return std::move(url);
}

OpenWeatherAPI::OpenWeatherAPI(QWidget* const parent, const QApplication& app) :
        parent(parent), app(app) {}

OpenWeatherAPI::~OpenWeatherAPI() {}

void OpenWeatherAPI::callAPI() {
        curl.fetchData(getUserURL());
        api_response = json::parse(curl.getResponse());
}

void OpenWeatherAPI::printResponse() const {
        if (api_response.empty()) {
                qWarning() << "API response is empty";
        } else {
                qInfo() << api_response.dump(8);
        }
}

const json& OpenWeatherAPI::getResponse() const {
        return api_response;
}

const QApplication& OpenWeatherAPI::getApp() const {
        return app;
}

const CurlHandler& OpenWeatherAPI::getCurl() const {
        return curl;
}
