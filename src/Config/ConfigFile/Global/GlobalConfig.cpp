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

#include "GlobalConfig.h"
#include "Config/ConfigFile/ConfigMapper.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>
#include <QSize>
#include <QSizePolicy>
#include <QString>

GlobalConfig::GlobalConfig(WindowProperties        window_properties,
                           PrimaryButtonProperties primary_button_properties) :
        window_properties(std::move(window_properties)),
        primary_button_properties(std::move(primary_button_properties)) {}

const WindowProperties& GlobalConfig::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& GlobalConfig::getPrimaryButtonProperties() const {
        return primary_button_properties;
}
