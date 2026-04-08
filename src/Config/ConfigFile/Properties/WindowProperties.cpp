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

#include "WindowProperties.h"

#include <utility>
#include <QSize>
#include <QString>

WindowProperties::WindowProperties(QSize size, QString title) :
        size(std::move(size)), title(std::move(title)) {}

const QString& WindowProperties::getTitle() const {
        return title;
}

const QSize& WindowProperties::getSize() const {
        return size;
}
