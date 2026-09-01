// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Map.h"

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Map/Helpers/Config.h"
#include "Core/Config/Map/Helpers/Keys.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/Config/View/View.h"

#include <QApplication>
#include <QStringView>
#include <toml++/toml.hpp>
#include <utility>

template<applet::Type TApplet>
config::schema::Config config::map::config(
        const toml::table& applet, const toml::table& global, const Config& defaults) {
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        QString path     = {};
        QString filename = "config.toml";

        ConfigView node = {node_view(applet), node_view(global), std::move(path),
                std::move(filename)};

        return {.window_params         = windowParams(node["window"], defaults.window_params),
                .primary_button_params = primaryButtonParams<TApplet>(
                        node["primary_button"], defaults.primary_button_params)};
}

template<applet::Type TApplet>
config::schema::Keys config::map::keys(const toml::table& applet, const toml::table& global,
        const Keys& defaults, keybindings& claimed_keys) {
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        QString path     = {};
        QString filename = "keys.toml";

        ConfigView node = {node_view(applet), node_view(global), std::move(path),
                std::move(filename)};

        return {.quit            = quit(node["quit"], defaults.quit, claimed_keys),
                .primary_buttons = primaryButtons(
                        node["primary_buttons"], defaults.primary_buttons, claimed_keys)};
}
