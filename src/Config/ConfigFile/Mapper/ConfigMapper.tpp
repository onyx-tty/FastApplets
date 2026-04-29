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

#include "Config/Resolvers/Resolvers.h"
#include "Config/TOML/Types/NodePair.h"
#include "ConfigMapper.h"
#include "CppUtils/include/Log/QtLog.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QStringLiteral>
#include <Qt>

template<typename TConfig>
void ConfigMapper::mapToPowerAppletConfig(const toml::table& power_applet_table,
                                          const toml::table& global_table, TConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { QFATAL("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TConfig::getDefault();
        QStringView filename = u"config.toml"_s;

        /* Window Properties */
        mapWindow(NodePair{power_applet_table["window"], global_table["window"]},
                  config.window_properties, defaults.getWindowProperties(),
                  PathContext{filename, u"window"_s});

        /* Primary Button Properties */
        mapPrimaryButton(NodePair{power_applet_table["primary_button"],
                                  global_table["primary_button"]},
                         config.primary_button_properties, defaults.getPrimaryButtonProperties(),
                         PathContext{filename, u"primary_button"_s});

        /* Layout Properties */
        mapLayout(power_applet_table["layout"], config.layout_properties,
                  defaults.getLayoutProperties(), PathContext{filename, u"layout"_s});

        /* Environment Properties */
        mapEnvironment(power_applet_table["environment"], config.environment_properties,
                       defaults.getEnvironmentProperties(),
                       PathContext{filename, u"environment"_s});
}
