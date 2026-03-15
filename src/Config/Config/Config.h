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

#include "Properties/WindowProperties.h"
#include "Properties/LayoutProperties.h"
#include "Properties/PrimaryButtonProperties.h"
#include "Properties/EnvironmentProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

class Config final {
public:
        Config(WindowProperties        window_properties         = {},
               PrimaryButtonProperties primary_button_properties = {},
               LayoutProperties        layout_properties         = {},
               EnvironmentProperties   environment_properties    = {});
        // TODO Make this const to avoid overwrites
        static Config&                 getConfig();
        static const Config&           getDefaultConfig();
        const WindowProperties&        getWindowProperties() const;
        const LayoutProperties&        getLayoutProperties() const;
        const PrimaryButtonProperties& getPrimaryButtonProperties() const;
        const EnvironmentProperties&   getEnvironmentProperties() const;

private:
        friend class ConfigMapper;

        WindowProperties        window_properties;
        PrimaryButtonProperties primary_button_properties;
        LayoutProperties        layout_properties;
        EnvironmentProperties   environment_properties;
};
