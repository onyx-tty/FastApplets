// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletType.h"
#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Types/ResolverCandidate.h"
#include "Config/Types/NodeView.h"
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

        TConfig config = TConfig{};

        ResolverCandidates cands = {{.node   = node_view(power_applet),
                                     .applet = applet::type::power_applet,
                                     .quiet  = true},
                                    {.node = node_view(global), .applet = applet::type::global}};

        /* Window Properties */
        // TODO: Use enum in .makeQuiet() to avoid magic numbers
        config.window_properties = window(cands.makeExtended("window").makeQuiet(false, 0),
                                          defaults.getWindowProperties(),
                                          PathContext{filename, u"window"_s});

        /* Primary Button Properties */
        config.primary_button_properties = primaryButton(cands.makeExtended("primary_button"),
                                                         defaults.getPrimaryButtonProperties(),
                                                         PathContext{filename, u"primary_button"_s});

        /* Layout Properties */
        config.layout_properties = layout({cands.get()[0].makeExtended("layout").makeQuiet(false)},
                                          defaults.getLayoutProperties(),
                                          PathContext{filename, u"layout"_s});

        return std::move(config);
}
