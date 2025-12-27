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

#include <QDebug>

namespace log_internals {

template<typename T>
QString toQString(const T& value);

// Concatenate all arguments into one string
inline QString concatArgs();

template<typename MainStr, typename... RestStr>
inline QString concatArgs(const MainStr& main_string, const RestStr&... rest_strings);

} // namespace log_internals

// Logging formatters
template<typename... Args>
inline void QFATAL(const Args&... args);

inline QDebug QCRITICAL();


inline QDebug QDEBUG();

inline QDebug QINFO();

#include "Log.tpp"
