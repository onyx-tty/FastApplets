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

template<typename... Args>
inline void QFATAL(const Args&... args) {
        qFatal("%s: %s", __func__, concatArgs(args...));
}

template<typename... Args>
inline void QCRITICAL(const Args&... args) {
        qCritical().nospace() << __func__ << ": " << concatArgs(args...);
}

template<typename... Args>
inline void QDEBUG(const Args&... args) {
        qDebug().nospace() << __func__ << ": " << concatArgs(args...);
}

template<typename... Args>
inline void QINFO(const Args&... args) {
        qInfo().nospace() << __func__ << ": " << concatArgs(args...);
}
