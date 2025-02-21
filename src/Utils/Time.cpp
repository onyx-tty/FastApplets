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

#include <QDebug>

#include <cstdlib>

using namespace std::chrono;

const time_t findMidnight() {
        // TODO Optimize
        const auto   current_clock_time = system_clock::now();
        const auto   current_raw_time   = system_clock::to_time_t(current_clock_time);
        const auto   current_localtime       = *localtime(&current_raw_time);
        const time_t current_unix_timestamp =
                duration_cast<seconds>(current_clock_time.time_since_epoch()).count();
        const time_t midnight_unix_timestamp = current_unix_timestamp
                                             - (current_localtime.tm_hour * 60 * 60)
                                             - (current_localtime.tm_min * 60) - current_localtime.tm_sec;

        return midnight_unix_timestamp;
}

const time_t findMidnight(const std::chrono::time_point<std::chrono::system_clock> timedate) {
        // TODO Optimize
        const auto   current_raw_time       = system_clock::to_time_t(timedate);
        const auto   current_time           = *localtime(&current_raw_time);
        const time_t current_unix_timestamp = duration_cast<seconds>(timedate.time_since_epoch())
                                                      .count();
        const time_t midnight_unix_timestamp = current_unix_timestamp
                                             - (current_time.tm_hour * 60 * 60)
                                             - (current_time.tm_min * 60) - current_time.tm_sec;

        return midnight_unix_timestamp;
}

const int findClosestHour() {
        const auto now_unprocessed = system_clock::to_time_t(system_clock::now());
        const auto now             = *localtime(&now_unprocessed);
        for (int first = 0; first + 3 <= 24; first = first + 3) {
                int second = first + 3;
                if (first <= now.tm_hour && now.tm_hour < second) {
                        // We convert hours to minutes before performing an operation
                        int current_minute = (now.tm_hour * 60) + now.tm_min;
                        int diff1          = std::abs(current_minute - (first * 60));
                        int diff2          = std::abs(current_minute - (second * 60));
                        return std::min(diff1, diff2);
                } else continue;
        }
        qFatal("Something went wrong because %s wasn't able to find the closest hour!", __func__);
};
