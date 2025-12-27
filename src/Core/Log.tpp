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

#pragma once

#include "Log.h"

// Handle all possible conversions to QString
template<typename T>
QString log_internals::toQString(const T& value) {
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

inline QString log_internals::concatArgs() {
        return "";
}

// Concatenate all arguments into one string
template<typename MainStr, typename... RestStr>
inline QString log_internals::concatArgs(const MainStr& main_string, const RestStr&... rest_strings) {
        return log_internals::toQString(main_string) + " " + log_internals::concatArgs(rest_strings...);
}

template<typename... Args>
inline void QFATAL(const Args&... args) {
        qFatal("%s: %s", __func__, log_internals::concatArgs(args...));
}

template<typename... Args>
inline void QCRITICAL(const Args&... args) {
        qCritical().nospace() << __func__ << ": " << log_internals::concatArgs(args...);
}

template<typename... Args>
inline void QDEBUG(const Args&... args) {
        qDebug().nospace() << __func__ << ": " << log_internals::concatArgs(args...);
}

template<typename... Args>
inline void QINFO(const Args&... args) {
        qInfo().nospace() << __func__ << ": " << log_internals::concatArgs(args...);
}
