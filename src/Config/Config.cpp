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
#include "ConfigMapper.h"
#include "ConfigParser.h"
#include "PrimaryButtonData.h"
#include "UI/Enums/ButtonIDs.h"

#include <toml++/toml.hpp>

/* Window Properties*/
Config::WindowProperties::WindowProperties(QSize size, QString title) : size(size), title(title) {}

const QString& Config::WindowProperties::getTitle() const {
        return title;
}

const QSize& Config::WindowProperties::getSize() const {
        return size;
}

/* Primary Button Properties */
Config::PrimaryButtonProperties::PrimaryButtonProperties(Qt::Alignment text_alignment,
                                                         Qt::Alignment icon_alignment,
                                                         QSize icon_size, QSizePolicy policy) :
        text_alignment(text_alignment), icon_alignment(icon_alignment), icon_size(icon_size),
        policy(policy) {}

const Qt::Alignment& Config::PrimaryButtonProperties::getTextAlignment() const {
        return text_alignment;
}

const Qt::Alignment& Config::PrimaryButtonProperties::getIconAlignment() const {
        return icon_alignment;
}

const QSize& Config::PrimaryButtonProperties::getIconSize() const {
        return icon_size;
}

const QSizePolicy& Config::PrimaryButtonProperties::getPolicy() const {
        return policy;
}

/* Window Layout Properties */
Config::WindowLayoutProperties::WindowLayoutProperties(
        std::vector<PrimaryButtonData> primary_power_buttons) :
        primary_power_buttons(primary_power_buttons) {}

const std::vector<PrimaryButtonData>& Config::WindowLayoutProperties::getPrimaryPowerButtons() const {
        return primary_power_buttons;
}

/* Environment Properties */
Config::EnvironmentProperties::EnvironmentProperties(bool dbus_mode) : dbus_mode(dbus_mode) {}

const bool& Config::EnvironmentProperties::getDBusMode() const {
        return dbus_mode;
}

/* Config */
Config::Config(Config::WindowProperties        window_properties,
               Config::PrimaryButtonProperties primary_button_properties,
               Config::WindowLayoutProperties  window_layout_properties,
               Config::EnvironmentProperties   environment_properties) :
        window_properties(std::move(window_properties)),
        window_layout_properties(std::move(window_layout_properties)),
        primary_button_properties(std::move(primary_button_properties)),
        environment_properties(std::move(environment_properties)) {}

Config& Config::getConfig() {
        static Config config{};
        static bool   parsed = false;

        if (!parsed) {
                ConfigMapper::mapToConfig(ConfigParser::createConfig(), config);
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
                {PrimaryButtonData{power_button_id::shutdown, "Shutdown", 1},
                 PrimaryButtonData{power_button_id::reboot, "Reboot", 2},
                 PrimaryButtonData{power_button_id::suspend, "Suspend", 3},
                 PrimaryButtonData{power_button_id::hibernate, "Hibernate", 4}};
        WindowLayoutProperties default_window_layout_properties = {std::move(primary_buttons)};

        bool                  dbus_mode = false;
        EnvironmentProperties default_environment_properties{dbus_mode};

        static Config default_config = {std::move(default_window_properties),
                                        std::move(default_primary_button_properties),
                                        std::move(default_window_layout_properties),
                                        std::move(default_environment_properties)};

        return default_config;
}

const Config::WindowProperties& Config::getWindowProperties() const {
        return window_properties;
}

const Config::WindowLayoutProperties& Config::getWindowLayoutProperties() const {
        return window_layout_properties;
}

const Config::PrimaryButtonProperties& Config::getPrimaryButtonProperties() const {
        return primary_button_properties;
}

const Config::EnvironmentProperties& Config::getEnvironmentProperties() const {
        return environment_properties;
}
