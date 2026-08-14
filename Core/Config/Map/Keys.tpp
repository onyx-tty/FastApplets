// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/Config/Types/PathContext/PathContext.h"
#include "Keys.h"

#include <toml++/toml.hpp>
#include <utility>
#include <QApplication>
#include <QDebug>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<applet::Type TApplet>
config::schema::Keys config::map::keys(const toml::table& applet, const toml::table& global,
                                       const Keys& defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"keys.toml";

        auto             keys  = Keys();
        const Candidates cands = {{.node         = node_view(applet),
                                   .applet       = TApplet,
                                   .quiet        = true,
                                   .path_context = PathContext(filename, u"")},
                                  {.node         = node_view(global),
                                   .applet       = applet::Type::Global,
                                   .quiet        = false,
                                   .path_context = PathContext(filename, u"")}};

        /* Quit Keys */
        keys.quit = quit(cands.makeCopy().withExtension(u"quit"), defaults.quit);

        /* Primary Button Keys */
        keys.primary_buttons = primaryButtons({cands[CandidateIndex::Applet]
                                                       .makeCopy()
                                                       .withExtension(u"primary_buttons")
                                                       .withQuiet(false)},
                                              defaults.primary_buttons);

        return std::move(keys);
}
