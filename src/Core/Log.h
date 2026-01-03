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
#include <QString>

namespace log_internals {

template<typename T>
QString toQString(const T& value);

// Concatenate all arguments into one string
inline QString concatArgs();

template<typename MainStr, typename... RestStr>
inline QString concatArgs(const MainStr& main_string, const RestStr&... rest_strings);

} // namespace log_internals

// TODO Log not only function names but also potential classes they belong to and perhaps even file names
// TODO Create versions with formatting
// Logging formatters
#define QFATAL(...) do { \
        const QString msg = log_internals::concatArgs(__VA_ARGS__); \
        qFatal("[FATAL]    %s : %s", __func__, qPrintable(msg)); \
} while(0)

#define QCRITICAL() \
        qCritical() << "[CRITICAL]" << __func__ << ":"

#define QWARNING() \
        qWarning() << "[WARNING]  " << __func__ << ":"

// TODO Optimize, this still instantiates a QDebug object
#ifdef LOG_DEBUG
        #define QDEBUG() \
                qDebug() << "[DEBUG]   " << __func__ << ":"
#else // Swallow anything streamed to it and ignore semicolons
        #define QDEBUG() \
                if (false) qDebug()
#endif

#define QINFO() \
        qInfo() << "[INFO]    " << __func__ << ":"

// Logging formatters - nospace version
#define QCRITICAL_NS() \
        qCritical().nospace() << "[CRITICAL]" << __func__ << " : "

#define QWARNING_NS() \
        qWarning().nospace() << "[WARNING]  " << __func__ << " : "

#define QDEBUG_NS() \
        qDebug().nospace() << "[DEBUG]   " << __func__ << " : "

#define QINFO_NS() \
        qInfo().nospace() << "[INFO]    " << __func__ << " : "

#include "Log.tpp"
