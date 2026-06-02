// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletRecord.h"
#include "Applets/Types/AppletTraits.h"
#include "Applets/Types/AppletType.h"
#include "Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/FileLocator/FileLocator.h"
#include "Config/KeysFile/Mapper/KeysMapper.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/TomlParser/TomlParser.h"
#include "Config/Types/ConfigFiles.h"
#include "ConfigManager.h"
#include "UI/Types/ButtonType.h"

#include <cassert>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

template<applet::type TApplet>
const ConfigFiles& ConfigManager<TApplet>::configFilepaths(applet::type applet) {
        switch (applet) {
        case applet::type::power_applet:
                static const ConfigFiles power = FileLocator::configFiles(applet::toString(applet));
                return power;
        case applet::type::global:
                static const ConfigFiles global = FileLocator::configFiles(applet::toString(applet));
                return global;
        default: assert(false && "Passed unknown type"); std::unreachable();
        }
}

template<applet::type TApplet>
ConfigManager<TApplet>::TConfig ConfigManager<TApplet>::makeDefaultConfig() {
        QSize   size  = {960, 220};
        QString title = QString::fromStdString(std::string(AppletTraits<TApplet>::title));

        WindowProperties window = WindowProperties{std::move(size), std::move(title)};

        Qt::Alignment           text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        Qt::Alignment           icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        QSize                   icon_size      = {64, 64};
        QSizePolicy             policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        PrimaryButtonProperties button{std::move(text_alignment), std::move(icon_alignment),
                                       std::move(icon_size), std::move(policy)};

        // TODO: At this point a builder class for config schemas would help a lot
        auto layout = LayoutProperties{};
        if constexpr (TApplet == applet::power_applet.type) {
                using enum power_button_type;
                std::vector<TPrimaryButtonParams> primary_buttons =
                        {TPrimaryButtonParams{.type    = shutdown,
                                              .text    = textFor(shutdown),
                                              .command = commandFor(shutdown),
                                              .icon    = iconFor(shutdown)},
                         TPrimaryButtonParams{.type    = reboot,
                                              .text    = textFor(reboot),
                                              .command = commandFor(reboot),
                                              .icon    = iconFor(reboot)},
                         TPrimaryButtonParams{.type    = suspend,
                                              .text    = textFor(suspend),
                                              .command = commandFor(suspend),
                                              .icon    = iconFor(suspend)},
                         TPrimaryButtonParams{.type    = hibernate,
                                              .text    = textFor(hibernate),
                                              .command = commandFor(hibernate),
                                              .icon    = iconFor(hibernate)}};

                layout = LayoutProperties(std::move(primary_buttons));
        }

        return {std::move(window), std::move(button), std::move(layout)};
}

template<applet::type TApplet>
ConfigManager<TApplet>::TKeys ConfigManager<TApplet>::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_button = {
                keybindings{Qt::Key_1}, keybindings{Qt::Key_2}, keybindings{Qt::Key_3},
                keybindings{Qt::Key_4}, keybindings{Qt::Key_5}, keybindings{Qt::Key_6},
                keybindings{Qt::Key_7}, keybindings{Qt::Key_8}, keybindings{Qt::Key_9},
        };

        return {std::move(quit), std::move(primary_button)};
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TConfig& ConfigManager<TApplet>::getConfig() {
        const auto& applet_files = configFilepaths(TApplet);
        const auto& global_files = configFilepaths(applet::type::global);

        static const TConfig config =
                ConfigMapper::config<TConfig>(TomlParser::file(applet_files.config),
                                              TomlParser::file(global_files.config),
                                              getDefaultConfig());

        return config;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TConfig& ConfigManager<TApplet>::getDefaultConfig() {
        static const TConfig default_config = makeDefaultConfig();

        return default_config;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TKeys& ConfigManager<TApplet>::getKeys() {
        const auto& applet_files = configFilepaths(TApplet);
        const auto& global_files = configFilepaths(applet::type::global);

        static TKeys keys = KeysMapper::keys<TKeys>(TomlParser::file(applet_files.keys),
                                                    TomlParser::file(global_files.keys),
                                                    getDefaultKeys());

        return keys;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TKeys& ConfigManager<TApplet>::getDefaultKeys() {
        static const TKeys default_keys = makeDefaultKeys();

        return default_keys;
}
