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

#include "PresentTime.h"
#include "../../../Utils/Time.h"
#include "../API/WeatherData.h"

#include <QApplication>
#include <QDebug>

time_t             PresentTimeManager::current_midnight;
time_t             PresentTimeManager::next_midnight;
int                PresentTimeManager::hour_spacing;
int                PresentTimeManager::blocs_per_day;
std::optional<int> PresentTimeManager::first_day_blocs;
int                PresentTimeManager::last_day_blocs;

// TODO Continue
void PresentTimeManager::refresh() {
        const auto iter_begin = WeatherData::hours.cbegin();
        const auto iter_end   = WeatherData::hours.cend();
        hour_spacing          = findHourSpacing((iter_begin + 1)->time, iter_begin->time);
        current_midnight      = findMidnight();
        next_midnight         = current_midnight + DAY;
        qInfo() << WeatherData::hours.size();
        qInfo() << "Hour spacing:" << hour_spacing;
        if (WeatherData::hours.empty() || WeatherData::hours.size() < 2) {
                qFatal("WeatherData::hours doesn't contain enough data! %zu",
                       WeatherData::hours.size());
                QApplication::quit();
        }
        // Because we start counting from 1 not 0
        if ((WeatherData::hours.size() - 1) % hour_spacing != 0) {
                qFatal("Hour spacing between each weather block is uneven! %zu %zu",
                       WeatherData::hours.size(), (WeatherData::hours.size() % hour_spacing));
                qFatal("This is not allowed!");
                QApplication::quit();
        }
        blocs_per_day = findWeatherBlocsFitCount(next_midnight, current_midnight, hour_spacing)
                                .value_or(0);
        first_day_blocs = findWeatherBlocsFitCount(next_midnight, iter_begin->time, hour_spacing)
                                  .value_or(0);
        // if no value is returned, assume that 0 has been received, otherwise accept the returned value
        if (first_day_blocs > blocs_per_day) {
                qFatal("First day blocs %i is higher than blocs per day %i! Not allowed! CWL",
                       first_day_blocs.value(), blocs_per_day);
                QApplication::quit();
        }
        last_day_blocs = blocs_per_day - first_day_blocs.value();
}

const int& PresentTimeManager::getBlocsPerDay() {
        if (current_midnight != findMidnight()) refresh();
        return blocs_per_day;
}

const std::optional<int>& PresentTimeManager::getFirstDayBlocs() {
        if (current_midnight != findMidnight()) refresh();
        return first_day_blocs;
}

const int& PresentTimeManager::getLastDayBlocs() {
        if (current_midnight != findMidnight()) refresh();
        return last_day_blocs;
}
