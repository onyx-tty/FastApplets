// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletConfig.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/FileLocator/FileLocator.h"
#include "Config/TomlParser/TomlParser.h"
#include "Config/Types/ConfigFiles.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

PowerAppletConfig::PowerAppletConfig(WindowProperties        window,
                                     PrimaryButtonProperties primary_button,
                                     LayoutProperties        layout) :
        GlobalConfig(std::move(window), std::move(primary_button)),
        layout_properties(std::move(layout)) {}

const PowerAppletConfig& PowerAppletConfig::get() {
        // TODO: Config files should not be fetched twice, once for config, once for keys.
        //       Either fetch them individually or cache the result for both.
        static const ConfigFiles power_files  = FileLocator::configFiles(applet::power_applet.name);
        static const ConfigFiles global_files = FileLocator::configFiles(applet::global.name);

        static const auto config =
                ConfigMapper::config<PowerAppletConfig>(TomlParser::file(power_files.config),
                                                        TomlParser::file(global_files.config));

        return config;
}

const PowerAppletConfig& PowerAppletConfig::getDefault() {
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

        static PowerAppletConfig config = {std::move(window), std::move(button), std::move(layout)};

        return config;
}

const LayoutProperties& PowerAppletConfig::getLayoutProperties() const {
        return layout_properties;
}
