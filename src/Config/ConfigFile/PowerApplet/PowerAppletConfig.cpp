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

PowerAppletConfig::PowerAppletConfig(WindowProperties        window_properties,
                                     PrimaryButtonProperties primary_button_properties,
                                     LayoutProperties        layout_properties) :
        GlobalConfig(std::move(window_properties), std::move(primary_button_properties)),
        layout_properties(std::move(layout_properties)) {}

PowerAppletConfig& PowerAppletConfig::get() {
        static PowerAppletConfig config{};
        static bool              parsed = false;

        if (!parsed) {
                // TODO: Config files should not be fetched twice, once for config, once for keys.
                //       Either fetch them individually or cache the result for both.
                ConfigFiles power_files = FileLocator::locateConfigFiles(applet::power_applet.scope);
                ConfigFiles global_files = FileLocator::locateConfigFiles(applet::global.scope);

                ConfigMapper::mapToPowerAppletConfig(TomlParser::parseFile(power_files.config),
                                                     TomlParser::parseFile(global_files.config),
                                                     config);
                parsed = true;
        }

        return config;
}

const PowerAppletConfig& PowerAppletConfig::getDefault() {
        QSize            size                      = {960, 220};
        QString          title                     = "PowerApplet";
        WindowProperties default_window_properties = WindowProperties{std::move(size),
                                                                      std::move(title)};

        Qt::Alignment           text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        Qt::Alignment           icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        QSize                   icon_size      = {64, 64};
        QSizePolicy             policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        PrimaryButtonProperties default_primary_button_properties{std::move(text_alignment),
                                                                  std::move(icon_alignment),
                                                                  std::move(icon_size),
                                                                  std::move(policy)};

        std::vector<PowerButtonParams> primary_buttons =
                {PowerButtonParams{.id      = power_button_type::shutdown,
                                   .text    = "Shutdown",
                                   .order   = 1,
                                   .command = "systemctl poweroff",
                                   .icon    = iconFor(power_button_type::shutdown)},
                 PowerButtonParams{.id      = power_button_type::reboot,
                                   .text    = "Reboot",
                                   .order   = 2,
                                   .command = "systemctl reboot",
                                   .icon    = iconFor(power_button_type::reboot)},
                 PowerButtonParams{.id      = power_button_type::suspend,
                                   .text    = "Suspend",
                                   .order   = 3,
                                   .command = "systemctl suspend",
                                   .icon    = iconFor(power_button_type::suspend)},
                 PowerButtonParams{.id      = power_button_type::hibernate,
                                   .text    = "Hibernate",
                                   .order   = 4,
                                   .command = "systemctl hibernate",
                                   .icon    = iconFor(power_button_type::hibernate)}};
        LayoutProperties default_layout_properties = LayoutProperties{std::move(primary_buttons)};

        static PowerAppletConfig default_power_applet_config =
                {std::move(default_window_properties), std::move(default_primary_button_properties),
                 std::move(default_layout_properties)};

        return default_power_applet_config;
}

const LayoutProperties& PowerAppletConfig::getLayoutProperties() const {
        return layout_properties;
}
