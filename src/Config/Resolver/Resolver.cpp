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

#include "Resolver.h"

#include <cstddef>
#include <string_view>
#include <QString>
#include <QStringView>

PathContext::PathContext(QStringView filename, QStringView path_context, char separator) :
        filename(filename.toString()), path_context(path_context.toString()), separator(separator) {
}

QString PathContext::makePath(std::string_view scope) const {
        return QString("in %1, %2%3%4")
                .arg(QString{filename}, QString::fromStdString(std::string{scope}))
                .arg(separator)
                .arg(path_context);
}

PathContext PathContext::getExtended(std::string_view segment) const {
        return PathContext{filename,
                           QString("%1%2%3")
                                   .arg(path_context)
                                   .arg(separator)
                                   .arg(QString::fromStdString(std::string{segment})),
                           separator};
}

PathContext PathContext::getExtended(size_t index) const {
        return PathContext{filename, QString("%1[%2]").arg(path_context).arg(index), separator};
}
