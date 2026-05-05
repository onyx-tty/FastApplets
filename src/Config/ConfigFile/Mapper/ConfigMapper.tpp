// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/Resolver.h"
#include "Config/Types/NodePair.h"
#include "ConfigMapper.h"
#include "CppUtils/Log/QtLog.h"

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
