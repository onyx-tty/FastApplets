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

#pragma once

#include <QApplication>
#include <QWidget>

#include <nlohmann/json.hpp>
#include <string>

class WeatherParser final {
public:
        explicit WeatherParser() = delete;

        static void updateWeatherData(const QApplication& app);

private:
        static void processWeatherItem(const std::string& key, const json& value,
                                       const std::string& path, int& index);
};
