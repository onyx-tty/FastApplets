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

#include "Time.h"

#include <QApplication>
#include <QDebug>

#include <cstdlib>

using namespace std::chrono;

time_t findCurrentUnixTime() {
        auto       now              = system_clock::now();
        const auto current_raw_time = system_clock::to_time_t(now);
        const auto current_time     = *gmtime(&current_raw_time);
        const time_t current_unix_timestamp = duration_cast<seconds>(now.time_since_epoch()).count();

        return current_unix_timestamp;
}

time_t findMidnight() {
        // TODO Optimize
        const auto   current_clock_time = system_clock::now();
        const auto   current_raw_time   = system_clock::to_time_t(current_clock_time);
        const auto   current_localtime  = *gmtime(&current_raw_time);
        const time_t current_unix_timestamp =
                duration_cast<seconds>(current_clock_time.time_since_epoch()).count();
        const time_t midnight_unix_timestamp = current_unix_timestamp
                                             - (current_localtime.tm_hour * 60 * 60)
                                             - (current_localtime.tm_min * 60)
                                             - current_localtime.tm_sec;

        return midnight_unix_timestamp;
}

// Used to dynamically predict the total numbers of indexes dedicated for each day
// i.e. if each index is spaced by 3 hours then we can expect 8 indexes/data blocs per day
int findHourSpacing(const time_t later, const time_t earlier) {
        if (later < earlier) {
                qCritical() << "Passed argument for \"later\" smaller than \"earlier\" in"
                            << __func__;
                QApplication::quit();
        }
        // hour in seconds
        constexpr int hour = 60 * 60;

        const int   time_difference = later - earlier;
        const div_t hour_spacing    = std::div(time_difference, hour);

        if (hour_spacing.rem != 0) {
                QString res = QString::number(hour_spacing.quot) + "r"
                            + QString::number(hour_spacing.rem);
                qWarning() << __func__ << "detected that hours aren't spaced evenly!"
                           << time_difference << "/" << hour << "=" << std::move(res);
        }

        return hour_spacing.quot;
}

// Used to predict how many weather blocs, or to be precise, spaced weather data elements can fit
// in a specified time range. If appropriate data is received, it should be processed smoothly.
// The quotient of remaining day time and hour spacing must be a whole number, otherwise it is
// implied that the timestamps have been tampered with or incorrectly passed.
// APIs are expected to pull data that is distributed evenly, meaning that there should be
// specific hour spacing.
// TODO Perhaps there is a safer way to ensure that our data cannot be tampered with along the way
std::optional<const int> findWeatherBlocsFitCount(const time_t later, const time_t earlier,
                                                  const int hour_spacing) {
        qDebug() << __func__ << "has received" << later << "as later and" << earlier
                 << "as earlier, spacing is" << hour_spacing;
        if (later < earlier) {
                qCritical() << "Passed argument for \"later\" smaller than \"earlier\" in"
                            << __func__;
                QApplication::quit();
        }
        // times in seconds
        constexpr time_t hour         = 60 * 60;
        constexpr time_t day          = hour * 24;
        const time_t     weather_bloc = hour * hour_spacing;

        const time_t time_elapsed = later - earlier;
        if (time_elapsed > day) {
                qFatal("Difference longer than one day! %s", __func__);
                QApplication::quit();
        }

        // find how many weather blocs can fit in the remaining time
        const div_t indexes = std::div((int) (time_elapsed / hour), (int) (weather_bloc / hour));
        if (indexes.rem != 0) { // if remainder found
                QString res = QString::number(indexes.quot) + "r" + QString::number(indexes.rem);
                qCritical() << __func__
                            << "detected that hours cannot be calculated spaced without loss!"
                            << "Timestamp corruption!" << time_elapsed << "/" << "(" << hour << "*"
                            << hour_spacing << ")"
                            << "=" << std::move(res);
                QApplication::quit();
        }

        qDebug() << "When parsed, we receive" << indexes.quot;
        return (indexes.quot != 0) ? std::optional(indexes.quot) : std::nullopt;
}

time_t findCloserHour(const time_t hour1, const time_t hour2) {
        const time_t now   = findCurrentUnixTime();
        const time_t diff1 = std::abs(hour1 - now);
        const time_t diff2 = std::abs(hour2 - now);

        return (diff1 > diff2) ? hour1 : hour2;
}
