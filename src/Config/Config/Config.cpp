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

#include "Config.h"
#include "Config/TOML/TomlParser.h"
#include "ConfigMapper.h"
#include "Properties/EnvironmentProperties.h"
#include "Properties/LayoutProperties.h"
#include "Properties/PrimaryButtonProperties.h"
#include "Properties/WindowProperties.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PrimaryButtonData.h"

#include <qnamespace.h>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>

#include <toml++/toml.hpp>

/* Config */
Config::Config(WindowProperties        window_properties,
               PrimaryButtonProperties primary_button_properties,
               LayoutProperties layout_properties, EnvironmentProperties environment_properties) :
        window_properties(std::move(window_properties)),
        primary_button_properties(std::move(primary_button_properties)),
        layout_properties(std::move(layout_properties)),
        environment_properties(std::move(environment_properties)) {}

Config& Config::getConfig() {
        static Config config{};
        static bool   parsed = false;

        if (!parsed) {
                ConfigMapper::mapToConfig(TomlParser::createConfig(), config);
                parsed = true;
        }

        return config;
}

const Config& Config::getDefaultConfig() {
        QSize            size                      = {960, 220};
        QString          title                     = "test_window";
        WindowProperties default_window_properties = {std::move(size), std::move(title)};

        Qt::Alignment           text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        Qt::Alignment           icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        QSize                   icon_size      = {64, 64};
        QSizePolicy             policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        PrimaryButtonProperties default_primary_button_properties{std::move(text_alignment),
                                                                  std::move(icon_alignment),
                                                                  std::move(icon_size),
                                                                  std::move(policy)};

        std::vector<PrimaryButtonData> primary_buttons =
                {PrimaryButtonData{power_button_id::shutdown,
                                   "Shutdown",
                                   1,
                                   {"systemctl", {"shutdown"}}},
                 PrimaryButtonData{power_button_id::reboot, "Reboot", 2, {"systemctl", {"reboot"}}},
                 PrimaryButtonData{power_button_id::suspend,
                                   "Suspend",
                                   3,
                                   {"systemctl", {"suspend"}}},
                 PrimaryButtonData{power_button_id::hibernate,
                                   "Hibernate",
                                   4,
                                   {"systemctl", {"hibernate"}}}};
        LayoutProperties default_layout_properties = {std::move(primary_buttons)};

        bool                  dbus_mode = false;
        EnvironmentProperties default_environment_properties{dbus_mode};

        static Config default_config = {std::move(default_window_properties),
                                        std::move(default_primary_button_properties),
                                        std::move(default_layout_properties),
                                        std::move(default_environment_properties)};

        return default_config;
}

const WindowProperties& Config::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& Config::getPrimaryButtonProperties() const {
        return primary_button_properties;
}

const LayoutProperties& Config::getLayoutProperties() const {
        return layout_properties;
}

const EnvironmentProperties& Config::getEnvironmentProperties() const {
        return environment_properties;
}
