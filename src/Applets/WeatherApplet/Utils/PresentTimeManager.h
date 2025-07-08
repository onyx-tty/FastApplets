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

#ifndef PRESENT_TIME_MANAGER_H
#define PRESENT_TIME_MANAGER_H

#include <ctime>
#include <optional>

// TODO Determine if std::optional is needed (if we need to differentiate "no input" from 0)
class PresentTimeManager {
public:
        static const int&                getBlocsPerDay();
        static const std::optional<int>& getFirstDayBlocs();
        static const int&                getLastDayBlocs();

        static void refresh();

private:
        static time_t             current_midnight;
        static time_t             next_midnight;
        static int                hour_spacing; // TODO Ideally should be const
        static int                blocs_per_day;
        static std::optional<int> first_day_blocs;
        static int                last_day_blocs;

        PresentTimeManager() = delete;
};

#endif // PRESENT_TIME_MANAGER_H
