// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Resolver/PathContext/PathContext.h"
#include "Core/Config/Resolver/Types/ResolverCandidate.h"
#include "Core/Config/Types/NodeView.h"
#include "KeysMapper.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QDebug>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<applet::type TApplet>
AppletTraits<TApplet>::TKeys KeysMapper::keys(const toml::table& applet, const toml::table& global,
                                              const AppletTraits<TApplet>::TKeys& defaults) {
        using TKeys = AppletTraits<TApplet>::TKeys;

        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        QStringView filename = u"keys.toml";

        TKeys                    keys = TKeys{};
        const ResolverCandidates cands =
                {{.node = node_view(applet), .applet = TApplet, .quiet = true},
                 {.node = node_view(global), .applet = applet::type::global}};

        /* Quit Keys */
        keys.quit = quit(cands.makeExtended("quit"), defaults.getQuit(),
                         PathContext{filename, u"quit"});

        /* Primary Button Keys */
        keys.primary_button =
                primaryButtons({cands.get()[0].makeExtended("primary_buttons").makeQuiet(false)},
                               defaults.getPrimaryButton(),
                               PathContext{filename, u"primary_buttons"});

        return std::move(keys);
}
