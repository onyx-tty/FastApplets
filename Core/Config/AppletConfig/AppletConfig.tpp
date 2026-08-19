// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletConfig.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/Defaults/Defaults.h"
#include "Core/Config/Map/Map.h"
#include "Core/Config/Toml/Parse.h"
#include "Core/Config/Types/Filepaths.h"

#include <QDebug>
#include <QtGlobal>

template<applet::Type TApplet>
requires(TApplet != applet::Type::Global)
config::applet_config config::makeAppletConfig(const Filepaths& applet, const Filepaths& global) {
        auto default_config = makeDefaultConfig<TApplet>();
        auto config         = map::config<TApplet>(
                parseTomlFile(applet.config), parseTomlFile(global.config), default_config);

        auto default_keys = makeDefaultKeys<TApplet>();
        auto keys         = map::keys<TApplet>(
                parseTomlFile(applet.keys), parseTomlFile(global.keys), default_keys);

        return applet_config(std::move(config), std::move(default_config), std::move(keys),
                std::move(default_keys));
}
