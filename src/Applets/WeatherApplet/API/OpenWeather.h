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

#ifndef OPEN_WEATHER_H
#define OPEN_WEATHER_H

#include "CurlHandler.h"

#include <QWidget>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class OpenWeatherAPI final {
private:
        QWidget*      parent;
        QApplication* app;
        CurlHandler   curl;
        std::string   dotenv_filepath; // TODO Move to a layout file
        std::string   api_key; // TODO Move to a layout file
        json          api_response;

        std::string userURL(std::string& api_key);
        QString     locateProjectRoot(const QString& marker);
        std::string fetchAPIKey();
        void        parseResponse();

public:
        explicit OpenWeatherAPI(QWidget* parent, QApplication* app);
        ~OpenWeatherAPI();

        void          printResponse() const;
        const QApplication* getApp() const;
        const CurlHandler&  getCurl() const;
};

#endif // OPEN_WEATHER_H
