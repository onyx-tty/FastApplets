// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigMapper.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Resolver/PathContext/PathContext.h"
#include "Core/Config/Resolver/Resolver.h"
#include "Core/Config/Resolver/Types/ResolverCandidate.h"
#include "Core/Config/Types/NodeView.h"

#include <toml++/toml.hpp>
#include <utility>
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<typename TConfig>
TConfig ConfigMapper::config(const toml::table& applet, const toml::table& global,
                             const TConfig& defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        QStringView filename = u"config.toml"_s;

        TConfig config = TConfig{};

        ResolverCandidates cands = {{.node   = node_view(applet),
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
