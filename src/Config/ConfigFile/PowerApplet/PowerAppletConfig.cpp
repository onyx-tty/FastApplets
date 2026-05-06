// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletConfig.h"
#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/TomlParser/TomlParser.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

PowerAppletConfig::PowerAppletConfig(WindowProperties        window_properties,
                                     PrimaryButtonProperties primary_button_properties,
                                     LayoutProperties        layout_properties,
                                     EnvironmentProperties   environment_properties) :
        GlobalConfig(std::move(window_properties), std::move(primary_button_properties)),
        layout_properties(std::move(layout_properties)),
        environment_properties(std::move(environment_properties)) {}

PowerAppletConfig& PowerAppletConfig::get() {
        static PowerAppletConfig power_applet_config{};
        static bool              parsed = false;

        if (!parsed) {
                ConfigMapper::mapToPowerAppletConfig(TomlParser::parsePowerAppletConfig(),
                                                     TomlParser::parseGlobalConfig(),
                                                     power_applet_config);
                parsed = true;
        }

        return power_applet_config;
}

const PowerAppletConfig& PowerAppletConfig::getDefault() {
        QSize            size                      = {960, 220};
        QString          title                     = "test_window";
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
                {PowerButtonParams{power_button_id::shutdown, "Shutdown", 1, "systemctl shutdown",
                                   dbusMethodFor(power_button_id::shutdown),
                                   iconFor(power_button_id::shutdown)},
                 PowerButtonParams{power_button_id::reboot, "Reboot", 2, "systemctl reboot",
                                   dbusMethodFor(power_button_id::reboot),
                                   iconFor(power_button_id::reboot)},
                 PowerButtonParams{power_button_id::suspend, "Suspend", 3, "systemctl suspend",
                                   dbusMethodFor(power_button_id::suspend),
                                   iconFor(power_button_id::suspend)},
                 PowerButtonParams{power_button_id::hibernate, "Hibernate", 4,
                                   "systemctl hibernate", dbusMethodFor(power_button_id::hibernate),
                                   iconFor(power_button_id::hibernate)}};
        LayoutProperties default_layout_properties = LayoutProperties{std::move(primary_buttons)};

        bool                  dbus_mode = false;
        EnvironmentProperties default_environment_properties{dbus_mode};

        static PowerAppletConfig default_power_applet_config =
                {std::move(default_window_properties), std::move(default_primary_button_properties),
                 std::move(default_layout_properties), std::move(default_environment_properties)};

        return default_power_applet_config;
}

const LayoutProperties& PowerAppletConfig::getLayoutProperties() const {
        return layout_properties;
}

const EnvironmentProperties& PowerAppletConfig::getEnvironmentProperties() const {
        return environment_properties;
}
