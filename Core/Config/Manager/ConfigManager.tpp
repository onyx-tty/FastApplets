// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigManager.h"
#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/Config/FileLocator/FileLocator.h"
#include "Core/Config/KeysFile/Mapper/KeysMapper.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Config/TomlParser/TomlParser.h"
#include "Core/Config/Types/ConfigFilepaths.h"
#include "Core/UI/Types/ButtonType.h"

#include <cassert>
#include <initializer_list>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

// TODO: TApplet is unused
// TODO: Remove switch, rely on template, it will generate a static for each template overload
template<applet::type TApplet>
const ConfigFilepaths& ConfigManager<TApplet>::configFilepaths(applet::type applet) {
        switch (applet) {
        case applet::type::power_applet:
                static const ConfigFilepaths power = FileLocator::configFiles(
                        applet::toLatin1String(applet));
                return power;
        case applet::type::action_applet:
                static const ConfigFilepaths action = FileLocator::configFiles(
                        applet::toLatin1String(applet));
        case applet::type::global:
                static const ConfigFilepaths global = FileLocator::configFiles(
                        applet::toLatin1String(applet));
                return global;
        // TODO: Use qFatal instead
        default: assert(false && "Passed unknown type"); std::unreachable();
        }
}

template<applet::type TApplet>
ConfigManager<TApplet>::TConfig ConfigManager<TApplet>::makeDefaultConfig() {
        QSize   size   = {960, 220};
        QString title  = QString::fromStdString(std::string(AppletTraits<TApplet>::title));
        auto    window = WindowProperties(std::move(size), std::move(title));

        Qt::Alignment text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        Qt::Alignment icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        QSize         icon_size      = {64, 64};
        QSizePolicy   policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        auto button = PrimaryButtonProperties(std::move(text_alignment), std::move(icon_alignment),
                                              std::move(icon_size), std::move(policy));

        // TODO: At this point a builder class for config schemas would help a lot
        if constexpr (TApplet == applet::type::power_applet) {
                // This has to be moved outside if more applets with primary button type
                // are created.
                using enum power_button_type;

                const auto param = [](power_button_type type) -> TPrimaryButtonParams {
                        return {.type    = type,
                                .text    = textFor(type),
                                .command = commandFor(type),
                                .icon    = iconFor(type)};
                };

                std::vector<TPrimaryButtonParams> params = {param(shutdown), param(reboot),
                                                            param(suspend), param(hibernate)};

                auto layout = LayoutProperties<typename AppletTraits<TApplet>::TPrimaryButtonParams>(
                        std::move(params));

                return TConfig{window, button, layout};
        }

        return TConfig{window, button};
}

template<applet::type TApplet>
ConfigManager<TApplet>::TKeys ConfigManager<TApplet>::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_buttons = makeKeyRange(Qt::Key_1, Qt::Key_9);

        return TKeys{std::move(quit), std::move(primary_buttons)};
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TConfig& ConfigManager<TApplet>::getConfig() {
        const auto& applet_files = configFilepaths(TApplet);
        const auto& global_files = configFilepaths(applet::type::global);

        static const TConfig config =
                ConfigMapper::config<TApplet>(TomlParser::file(applet_files.config),
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

        static TKeys keys = KeysMapper::keys<TApplet>(TomlParser::file(applet_files.keys),
                                                      TomlParser::file(global_files.keys),
                                                      getDefaultKeys());

        return keys;
}

template<applet::type TApplet>
const ConfigManager<TApplet>::TKeys& ConfigManager<TApplet>::getDefaultKeys() {
        static const TKeys default_keys = makeDefaultKeys();

        return default_keys;
}
