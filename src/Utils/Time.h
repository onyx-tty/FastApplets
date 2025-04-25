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

#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <ctime>
#include <optional>

time_t findCurrentUnixTime();
time_t findMidnight();
time_t findMidnight(const std::chrono::time_point<std::chrono::system_clock> timedate);
int    findHourSpacing(const time_t later, const time_t earlier);
std::optional<const int> findWeatherBlocsFitCount(const time_t later, const time_t earlier,
                                                  const int hour_spacing);
time_t                   findCloserHour(const time_t hour1, const time_t hour2); 

#endif // TIME_H
