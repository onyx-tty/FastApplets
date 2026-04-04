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

#pragma once

#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Config/Config/Properties/WindowProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

class GlobalConfig {
protected:
        friend class ConfigMapper;

        explicit GlobalConfig(
                WindowProperties        window_properties         = WindowProperties{},
                PrimaryButtonProperties primary_button_properties = PrimaryButtonProperties{});

        WindowProperties        window_properties;
        PrimaryButtonProperties primary_button_properties;

public:
        const WindowProperties&        getWindowProperties() const;
        const PrimaryButtonProperties& getPrimaryButtonProperties() const;
};
