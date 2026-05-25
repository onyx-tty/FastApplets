// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Types/ResolverCandidate.h"
#include "ConfigMapper.h"
#include "CppUtils/Log/QtLog.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QStringView>
#include <Qt>

template<typename TConfig>
TConfig ConfigMapper::config(const toml::table& power_applet, const toml::table& global) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { QFATAL("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TConfig::getDefault();
        QStringView filename = u"config.toml"_s;

        TConfig config{};

        /* Window Properties */
        config.window_properties = window({ResolverCandidate{.node   = power_applet["window"],
                                                             .applet = applet::type::power_applet},
                                           ResolverCandidate{.node   = global["window"],
                                                             .applet = applet::type::global}},
                                          defaults.getWindowProperties(),
                                          PathContext{filename, u"window"_s});

        /* Primary Button Properties */
        config.primary_button_properties =
                primaryButton({ResolverCandidate{.node   = power_applet["primary_button"],
                                                 .applet = applet::type::power_applet},
                               ResolverCandidate{.node   = global["primary_button"],
                                                 .applet = applet::type::global}},
                              defaults.getPrimaryButtonProperties(),
                              PathContext{filename, u"primary_button"_s});

        /* Layout Properties */
        config.layout_properties = layout(power_applet["layout"], defaults.getLayoutProperties(),
                                          PathContext{filename, u"layout"_s});

        return std::move(config);
}
