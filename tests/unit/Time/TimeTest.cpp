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

#include "Time/Time.h"
#include "Time/TimeConstants.h"

#include <gtest/gtest.h>
#include <cmath>

// Check if each was taken before and after midnight respectively
// If yes, the unit test may have been ran during midnight
bool dayDiffers(const size_t& t1, const size_t& t2) {
        return (t1 - t2) > epoch_duration::day;
}

TEST(FindCurrentUnixTimeTest, IsNotNegative) {
        EXPECT_GT(findCurrentUnixTime(), 0);
}

TEST(FindCurrentUnixTimeTest, MatchesSystemTime) {
        const time_t t1 = findCurrentUnixTime();
        const time_t t2 = time(nullptr);

        EXPECT_LE(std::abs(t1 - t2), 2); // allow 2 seconds of difference
}

TEST(FindCurrentUnixTimeTest, IsMonotonic) {
        const time_t t1 = findCurrentUnixTime();
        sleep(2); // TODO Multithreading
        const time_t t2 = findCurrentUnixTime();

        EXPECT_LE(t1, t2);
}

TEST(FindMidnightTest, IsNotNegative) {
        EXPECT_GT(findCurrentUnixTime(), 0);
}

TEST(FindMidnightTest, TwoCallsReturnSameValueWithinSameDay) {
        const time_t start = findCurrentUnixTime();
        time_t t1 = findMidnight();
        time_t t2 = findMidnight();
        const time_t end = findCurrentUnixTime();

        if (dayDiffers(start, end)) {
                t1 = findMidnight();
                t2 = findMidnight();
        }

        EXPECT_EQ(t1, t2);
}

// TODO TwoCallsSpacedBy24HoursIfCalledOnDifferentDays
// t1 = findMidnight(), t2 = findMidnight() + epoch_duration::day
// requires params

// TODO Simplify or break down findHourSpacing to make it readable and testable

// TODO Simplify or break down findWeatherBlocsFitCount to make it readable and testable

TEST(FindCloserHourTest, Hour1IsCloser) {
        const time_t hour1 = time(nullptr);
        const time_t hour2 = 555;
        const time_t closer = findCloserHour(hour1, hour2);

        EXPECT_EQ(closer, hour1);
}

TEST(FindCloserHourTest, Hour2IsCloser) {
        const time_t hour1 = 555;
        const time_t hour2 = time(nullptr);
        const time_t closer = findCloserHour(hour1, hour2);

        EXPECT_EQ(closer, hour2);
}

TEST(TimeInteractionTest, CurrentUnixTimeAndMidnightAreSameDay) {
        time_t t1 = findCurrentUnixTime();
        time_t t2 = findMidnight();

        // TODO Multithreading
        if (dayDiffers(t1, t2)) {
                t1 = findCurrentUnixTime();
                t2 = findMidnight();
        }

        EXPECT_LE((t1 - t2), epoch_duration::day);
}
