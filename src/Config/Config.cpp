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

#include <toml++/toml.hpp>

using std::vector;

/* Window Properties*/
Config::WindowProperties::WindowProperties(QSize size, QString title) : size(size), title(title) {}

const QString& Config::WindowProperties::getTitle() const {
        return title;
}

const QSize& Config::WindowProperties::getSize() const {
        return size;
}

/* Window Layout Properties */
Config::WindowLayoutProperties::WindowLayoutProperties(
        std::vector<PrimaryButtonData> primary_power_buttons) :
        primary_power_buttons(primary_power_buttons) {}

const vector<PrimaryButtonData>& Config::WindowLayoutProperties::getPrimaryPowerButtons() const {
        return primary_power_buttons;
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

/* Config */
Config::Config(Config::WindowProperties        window_properties,
               Config::WindowLayoutProperties  window_layout_properties,
               Config::PrimaryButtonProperties primary_button_properties) :
        window_properties(std::move(window_properties)),
        window_layout_properties(std::move(window_layout_properties)),
        primary_button_properties(std::move(primary_button_properties)) {}

Config& Config::getConfig() {
        static Config config{};
        static bool   parsed = false;

        if (!parsed) {
                ConfigMapper::mapToConfig(ConfigParser::createConfig(), config);
                parsed = true;
        }

        return config;
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
