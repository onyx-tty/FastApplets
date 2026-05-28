// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletType.h"
#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Types/ResolverCandidate.h"
#include "Config/Types/NodeView.h"
#include "KeysMapper.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QDebug>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<typename TKeys>
TKeys KeysMapper::keys(const toml::table& power_applet, const toml::table& global) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TKeys::getDefault();
        QStringView filename = u"keys.toml"_s;

        TKeys                    keys  = TKeys{};
        const ResolverCandidates cands = {{.node   = node_view(power_applet),
                                           .applet = applet::type::power_applet,
                                           .quiet  = true},
                                          {.node   = node_view(global),
                                           .applet = applet::type::global}};

        /* Quit Keys */
        keys.quit = quit(cands.makeExtended("quit"), defaults.getQuit(),
                         PathContext{filename, u"quit"_s});

        /* Primary Button Keys */
        keys.primary_button =
                primaryButtons({cands.get()[0].makeExtended("primary_buttons").makeQuiet(false)},
                               defaults.getPrimaryButton(),
                               PathContext{filename, u"primary_buttons"_s});

        return std::move(keys);
}
