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

#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

class PowerAppletConfig final : public GlobalConfig {
private:
        friend class ConfigMapper;

        LayoutProperties      layout_properties;
        EnvironmentProperties environment_properties;

public:
        PowerAppletConfig(
                WindowProperties        window_properties         = WindowProperties{},
                PrimaryButtonProperties primary_button_properties = PrimaryButtonProperties{},
                LayoutProperties        layout_properties         = LayoutProperties{},
                EnvironmentProperties   environment_properties    = EnvironmentProperties{});
        // TODO Make this const to avoid overwrites
        static PowerAppletConfig&       get();
        static const PowerAppletConfig& getDefault();
        const LayoutProperties&         getLayoutProperties() const;
        const EnvironmentProperties&    getEnvironmentProperties() const;
};
