// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Types/NodePair.h"
#include "ConfigMapper.h"
#include "CppUtils/Log/QtLog.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QStringView>
#include <Qt>

template<typename TConfig>
void ConfigMapper::mapToPowerAppletConfig(const toml::table& power_applet,
                                          const toml::table& global, TConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { QFATAL("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TConfig::getDefault();
        QStringView filename = u"config.toml"_s;

        /* Window Properties */
        mapWindow(NodePair{.primary = power_applet["window"], .fallback = global["window"]},
                  config.window_properties, defaults.getWindowProperties(),
                  PathContext{filename, u"window"_s});

        /* Primary Button Properties */
        mapPrimaryButton(NodePair{.primary  = power_applet["primary_button"],
                                  .fallback = global["primary_button"]},
                         config.primary_button_properties, defaults.getPrimaryButtonProperties(),
                         PathContext{filename, u"primary_button"_s});

        /* Layout Properties */
        mapLayout(power_applet["layout"], config.layout_properties, defaults.getLayoutProperties(),
                  PathContext{filename, u"layout"_s});
}
