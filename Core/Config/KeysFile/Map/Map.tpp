// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/KeysFile/Schema/Keys.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Types/Candidate.h"
#include "Core/Config/Types/NodeView.h"
#include "Map.h"

#include <toml++/toml.hpp>
#include <utility>
#include <QApplication>
#include <QDebug>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<applet::type TApplet>
config::schema::Keys config::map::keys(const toml::table& applet, const toml::table& global,
                                       const Keys& defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"keys.toml";

        auto             keys  = Keys();
        const Candidates cands = {{.node = node_view(applet), .applet = TApplet, .quiet = true},
                                  {.node = node_view(global), .applet = applet::type::global}};

        /* Quit Keys */
        keys.quit = quit(cands.makeCopy().withExtension("quit"), defaults.quit,
                         PathContext(filename, u"quit"));

        /* Primary Button Keys */
        keys.primary_buttons = primaryButtons(
                {cands.get()[0].makeCopy().withExtension("primary_buttons").withQuiet(false)},
                defaults.primary_buttons, PathContext(filename, u"primary_buttons"));

        return std::move(keys);
}
