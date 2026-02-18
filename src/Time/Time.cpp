/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

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

#include "Time.h"
#include "Core/Log.h"

#include <cstdlib>
#include <QApplication>

time_t findCurrentUnixTime() {
        auto         now              = std::chrono::system_clock::now();
        const auto   current_raw_time = std::chrono::system_clock::to_time_t(now);
        const auto   current_time     = *gmtime(&current_raw_time);
        const time_t current_unix_timestamp =
                duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

        return current_unix_timestamp;
}

time_t findMidnight() {
        // TODO Optimize
        const auto   current_clock_time = std::chrono::system_clock::now();
        const auto   current_raw_time   = std::chrono::system_clock::to_time_t(current_clock_time);
        const auto   current_localtime  = *gmtime(&current_raw_time);
        const time_t current_unix_timestamp =
                duration_cast<std::chrono::seconds>(current_clock_time.time_since_epoch()).count();
        const time_t midnight_unix_timestamp = current_unix_timestamp
                                             - (current_localtime.tm_hour * 60 * 60)
                                             - (current_localtime.tm_min * 60)
                                             - current_localtime.tm_sec;

        return midnight_unix_timestamp;
}

time_t findCloserHour(time_t hour1, time_t hour2) {
        const time_t now   = findCurrentUnixTime();
        const time_t diff1 = std::abs(hour1 - now);
        const time_t diff2 = std::abs(hour2 - now);

        return (diff1 <= diff2) ? hour1 : hour2;
}
