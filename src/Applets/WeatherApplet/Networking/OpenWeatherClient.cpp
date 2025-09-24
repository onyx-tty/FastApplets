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

#include "OpenWeatherClient.h"
#include "../../../../modules/CurlOOP/include/CurlOOP.h"
#include "../../../Config/WeatherLayout.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <string>

using nlohmann::json;

json OpenWeatherClient::fetchWeatherReport() {
        // Build target API call URL from API key and target city expressed as code
        static std::string api_call_url = WeatherLayoutManager::env_prop.getAPICallURL();

        // Return our weather report
        return json::parse(CurlOOP::download(api_call_url));
}
