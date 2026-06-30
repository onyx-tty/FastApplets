// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
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
Keys KeysMapper::keys(const toml::table& applet, const toml::table& global, const Keys& defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        QStringView filename = u"keys.toml";

        Keys                     keys = Keys{};
        const ResolverCandidates cands =
                {{.node = node_view(applet), .applet = TApplet, .quiet = true},
                 {.node = node_view(global), .applet = applet::type::global}};

        /* Quit Keys */
        keys.quit = quit(cands.makeExtended("quit"), defaults.getQuit(),
                         PathContext{filename, u"quit"});

        /* Primary Button Keys */
        keys.primary_buttons =
                primaryButtons({cands.get()[0].makeExtended("primary_buttons").makeQuiet(false)},
                               defaults.getPrimaryButtons(),
                               PathContext{filename, u"primary_buttons"});

        return std::move(keys);
}
