// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigManager.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/FileLocator/FileLocator.h"
#include "Config/KeysFile/Mapper/KeysMapper.h"
#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/TomlParser/TomlParser.h"
#include "Config/Types/ConfigFiles.h"
#include "UI/Types/ButtonType.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

// TODO: Consider noexcept to avoid incomplete statics
PowerAppletConfig ConfigManager::makeDefaultConfig() {
        QSize            size   = {960, 220};
        QString          title  = "PowerApplet";
        WindowProperties window = WindowProperties{std::move(size), std::move(title)};

        Qt::Alignment           text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        Qt::Alignment           icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        QSize                   icon_size      = {64, 64};
        QSizePolicy             policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        PrimaryButtonProperties button{std::move(text_alignment), std::move(icon_alignment),
                                       std::move(icon_size), std::move(policy)};

        using enum power_button_type;
        std::vector<PowerButtonParams>
                         primary_buttons{PowerButtonParams{.type    = shutdown,
                                                           .text    = textFor(shutdown),
                                                           .command = commandFor(shutdown),
                                                           .icon    = iconFor(shutdown)},
                                         PowerButtonParams{.type    = reboot,
                                                           .text    = textFor(reboot),
                                                           .command = commandFor(reboot),
                                                           .icon    = iconFor(reboot)},
                                         PowerButtonParams{.type    = suspend,
                                                           .text    = textFor(suspend),
                                                           .command = commandFor(suspend),
                                                           .icon    = iconFor(suspend)},
                                         PowerButtonParams{.type    = hibernate,
                                                           .text    = textFor(hibernate),
                                                           .command = commandFor(hibernate),
                                                           .icon    = iconFor(hibernate)}};
        LayoutProperties layout = LayoutProperties{std::move(primary_buttons)};

        return {std::move(window), std::move(button), std::move(layout)};
}

PowerAppletKeys ConfigManager::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_button = {
                keybindings{Qt::Key_1}, keybindings{Qt::Key_2}, keybindings{Qt::Key_3},
                keybindings{Qt::Key_4}, keybindings{Qt::Key_5}, keybindings{Qt::Key_6},
                keybindings{Qt::Key_7}, keybindings{Qt::Key_8}, keybindings{Qt::Key_9},
        };

        static PowerAppletKeys keys{std::move(quit), std::move(primary_button)};

        return std::move(keys);
}

void ConfigManager::init() {
        if (initialized) {
                qWarning() << "Configs are already initialized";
                return;
        }

        // TODO: Replace ConfigFiles with an array and a loop to avoid duplicate logic
        static const ConfigFiles power_files  = FileLocator::configFiles(applet::power_applet.name);
        static const ConfigFiles global_files = FileLocator::configFiles(applet::global.name);

        default_config = makeDefaultConfig();
        config = ConfigMapper::config<PowerAppletConfig>(TomlParser::file(power_files.config),
                                                         TomlParser::file(global_files.config),
                                                         default_config);

        default_keys = makeDefaultKeys();
        keys = KeysMapper::keys<PowerAppletKeys>(TomlParser::file(power_files.keys),
                                                 TomlParser::file(global_files.keys), default_keys);

        initialized = true;
}

// TODO: Remove boilerplate
const PowerAppletConfig& ConfigManager::getConfig() {
        if (!initialized) { qFatal("Called before init()!"); }

        return config;
}

const PowerAppletConfig& ConfigManager::getDefaultConfig() {
        if (!initialized) { qFatal("Called before init()!"); }

        return default_config;
}

const PowerAppletKeys& ConfigManager::getKeys() {
        if (!initialized) { qFatal("Called before init()!"); }

        return keys;
}

const PowerAppletKeys& ConfigManager::getDefaultKeys() {
        if (!initialized) { qFatal("Called before init()!"); }

        return default_keys;
}
