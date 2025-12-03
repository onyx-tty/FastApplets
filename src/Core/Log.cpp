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

#include "Log.h"

// Handle all possible conversions to QString
template<typename T>
static QString toQString(const T& value) {
        if constexpr (std::is_arithmetic_v<T>) {
                return QString::number(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
                return QString::fromStdString(value.c_str());
        } else if constexpr (std::is_convertible_v<T, QString>) {
                return QString(value);
        } else {
                static_assert(!sizeof(T*), "Type not supported for toQString!");
        }
}

// Concatenate all arguments into one string
static inline QString concatArgs() { return ""; }

template<typename MainStr, typename... RestStr>
static inline QString concatArgs(const MainStr& main_string, const RestStr&... rest_strings) {
        return toQString(main_string) + " " + concatArgs(rest_strings...);
}
