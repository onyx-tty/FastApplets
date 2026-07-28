// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletConfig.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Core/Config/Defaults/Defaults.h"
#include "Core/Config/KeysFile/Mapper/KeysMapper.h"
#include "Core/Config/Toml/Parse.h"
#include "Core/Config/Types/Filepaths.h"

#include <tuple>
#include <QDebug>
#include <QtGlobal>

template<applet::type TApplet>
requires(TApplet != applet::type::global)
config::AppletConfig config::makeAppletConfig(const Filepaths& applet, const Filepaths& global) {
        auto cfg = AppletConfig();

        cfg.default_config = makeDefaultConfig<TApplet>();
        cfg.config         = ConfigMapper::config<TApplet>(parseTomlFile(applet.config),
                                                           parseTomlFile(global.config),
                                                           cfg.default_config);

        cfg.default_keys = makeDefaultKeys<TApplet>();
        cfg.keys = KeysMapper::keys<TApplet>(parseTomlFile(applet.keys), parseTomlFile(global.keys),
                                             cfg.default_keys);

        return cfg;
}

template<applet::type TApplet>
requires(TApplet != applet::type::global)
auto config::makeAppletConfigTuple(const Filepaths& applet, const Filepaths& global) {
        auto cfg = makeAppletConfig<TApplet>(applet, global);

        return std::make_tuple(std::move(cfg.config), std::move(cfg.default_config),
                               std::move(cfg.keys), std::move(cfg.default_keys));
}
