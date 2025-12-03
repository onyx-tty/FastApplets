/* FastApplets

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

#include "ForecastBlocManager.h"
#include "../../../Time/Time.h"
#include "../../../Time/TimeConstants.h"
#include "../Data/WeatherData.h"

#include <QDebug>

time_t             ForecastBlocManager::current_midnight;
time_t             ForecastBlocManager::next_midnight;
int                ForecastBlocManager::hour_spacing;
int                ForecastBlocManager::blocs_per_day;
std::optional<int> ForecastBlocManager::first_day_blocs;
int                ForecastBlocManager::last_day_blocs;

// TODO Continue
void ForecastBlocManager::refresh() {
        const auto begin = WeatherData::hours.cbegin(), end = WeatherData::hours.cend();
        hour_spacing     = findHourSpacing((begin + 1)->time, begin->time);
        current_midnight = findMidnight();
        next_midnight    = current_midnight + epoch_duration::day;
        qDebug() << "WeatherData::hours size:" << WeatherData::hours.size() << "in" << __func__;
        qDebug() << "Hour spacing:" << hour_spacing << "in" << __func__;
        if (WeatherData::hours.empty() || WeatherData::hours.size() < 2) {
                qFatal("WeatherData::hours doesn't contain enough data! %zu %s",
                       WeatherData::hours.size(), __func__);
        }
        // Because we start counting from 1 not 0
        if ((WeatherData::hours.size() - 1) % hour_spacing != 0) {
                qFatal("Hour spacing between each weather block is uneven! %zu %zu %s",
                       WeatherData::hours.size(), (WeatherData::hours.size() % hour_spacing),
                       __func__);
        }
        blocs_per_day = findWeatherBlocsFitCount(next_midnight, current_midnight, hour_spacing)
                                .value_or(0);
        first_day_blocs = findWeatherBlocsFitCount(next_midnight, begin->time, hour_spacing)
                                  .value_or(0);
        // if no value is returned, assume that 0 has been received, otherwise accept the returned value
        if (first_day_blocs > blocs_per_day) {
                qFatal("First day blocs %i is higher than blocs per day %i! Not allowed! %s",
                       first_day_blocs.value(), blocs_per_day, __func__);
        }
        last_day_blocs = blocs_per_day - first_day_blocs.value();
}

const int& ForecastBlocManager::getBlocsPerDay() {
        if (current_midnight != findMidnight()) refresh();
        return blocs_per_day;
}

const std::optional<int>& ForecastBlocManager::getFirstDayBlocs() {
        if (current_midnight != findMidnight()) refresh();
        return first_day_blocs;
}

const int& ForecastBlocManager::getLastDayBlocs() {
        if (current_midnight != findMidnight()) refresh();
        return last_day_blocs;
}
