/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "PowerAppletConfig.h"
#include "Config/ConfigFile/ConfigMapper.h"
#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/TOML/TomlParser.h"
#include "Log/Log.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <qnamespace.h>
#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>

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
                {PowerButtonParams{power_button_id::shutdown,
                                   "Shutdown",
                                   1,
                                   {"systemctl", {"shutdown"}},
                                   dbusMethodFor(power_button_id::shutdown),
                                   iconFor(power_button_id::shutdown)},
                 PowerButtonParams{power_button_id::reboot,
                                   "Reboot",
                                   2,
                                   {"systemctl", {"reboot"}},
                                   dbusMethodFor(power_button_id::reboot),
                                   iconFor(power_button_id::reboot)},
                 PowerButtonParams{power_button_id::suspend,
                                   "Suspend",
                                   3,
                                   {"systemctl", {"suspend"}},
                                   dbusMethodFor(power_button_id::suspend),
                                   iconFor(power_button_id::suspend)},
                 PowerButtonParams{power_button_id::hibernate,
                                   "Hibernate",
                                   4,
                                   {"systemctl", {"hibernate"}},
                                   dbusMethodFor(power_button_id::hibernate),
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
