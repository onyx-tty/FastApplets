// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Map.h"

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Map/Helpers/Config.h"
#include "Core/Config/Map/Helpers/Helpers.h"
#include "Core/Config/Map/Helpers/Keys.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/Config/Types/PathContext/PathContext.h"
#include "Core/UI/Types/ButtonType.h"

#include <QApplication>
#include <QStringView>
#include <toml++/toml.hpp>

template<applet::Type TApplet>
config::schema::Config config::map::config(
        const toml::table& applet, const toml::table& global, const Config& defaults) {
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"config.toml";

        Candidate c = {.node  = node_view(applet),
                .applet       = TApplet,
                .quiet        = true,
                .path_context = PathContext(filename, u"")};

        Candidates cands = {
                {.node                = node_view(applet),
                 .applet       = TApplet,
                 .quiet        = true,
                 .path_context = PathContext(filename, u"")},
                {.node                = node_view(global),
                 .applet       = applet::Type::Global,
                 .quiet        = false,
                 .path_context = PathContext(filename, u"")}
        };

        return table<Config>(cands, [&defaults, &cands](Config& config) {
                config.window_params = windowParams(
                        cands.makeCopy().withExtension(u"window"), defaults.window_params);

                config.primary_button_params = primaryButtonParams<TApplet>(
                        cands.makeCopy().withExtension(u"primary_button"),
                        defaults.primary_button_params);
        }).value_or(defaults);
}

template<applet::Type TApplet>
config::schema::Keys config::map::keys(
        const toml::table& applet, const toml::table& global, const Keys& defaults) {
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"keys.toml";

        auto             keys  = Keys();
        const Candidates cands = {
                {.node                = node_view(applet),
                 .applet       = TApplet,
                 .quiet        = true,
                 .path_context = PathContext(filename, u"")},
                {.node                = node_view(global),
                 .applet       = applet::Type::Global,
                 .quiet        = false,
                 .path_context = PathContext(filename, u"")}
        };

        return table<Keys>(cands, [&defaults, &cands](Keys& keys) {
                keys.quit = quit(cands.makeCopy().withExtension(u"quit"), defaults.quit);

                keys.primary_buttons = primaryButtons(
                        {cands[CandidateIndex::Applet]
                                        .makeCopy()
                                        .withExtension(u"primary_buttons")
                                        .withQuiet(false)},
                        defaults.primary_buttons);
        }).value_or(defaults);
}
