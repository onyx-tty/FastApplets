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

#include "PrimaryButtonProperties.h"

#include <qnamespace.h>
#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>

PrimaryButtonProperties::PrimaryButtonProperties(Qt::Alignment text_alignment,
                                                 Qt::Alignment icon_alignment, QSize icon_size,
                                                 QSizePolicy policy) :
        text_alignment(text_alignment), icon_alignment(icon_alignment), icon_size(icon_size),
        policy(policy) {}

const Qt::Alignment& PrimaryButtonProperties::getTextAlignment() const {
        return text_alignment;
}

const Qt::Alignment& PrimaryButtonProperties::getIconAlignment() const {
        return icon_alignment;
}

const QSize& PrimaryButtonProperties::getIconSize() const {
        return icon_size;
}

const QSizePolicy& PrimaryButtonProperties::getPolicy() const {
        return policy;
}
