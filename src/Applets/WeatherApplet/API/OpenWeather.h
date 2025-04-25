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

#ifndef OPEN_WEATHER_H
#define OPEN_WEATHER_H

#include "CurlHandler.h"

#include <QWidget>

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class OpenWeatherAPI final {
private:
        QWidget* const      parent;
        const QApplication& app;
        CurlHandler         curl;
        json                api_response;

        const std::string getUserURL();

public:
        explicit OpenWeatherAPI(QWidget* const parent, const QApplication& app);
        ~OpenWeatherAPI();

        void                callAPI();
        void                printResponse() const;
        const json&         getResponse() const;
        const QApplication& getApp() const;
        const CurlHandler&  getCurl() const;
};

#endif // OPEN_WEATHER_H
