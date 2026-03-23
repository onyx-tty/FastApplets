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

#include "GlobalConfig.h"
#include "Config/Config/ConfigMapper.h"
#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Config/Config/Properties/WindowProperties.h"
#include "Config/TOML/TomlParser.h"

#include <qnamespace.h>
#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>

/* Global Config */
GlobalConfig::GlobalConfig(WindowProperties        window_properties,
                           PrimaryButtonProperties primary_button_properties) :
        window_properties(std::move(window_properties)),
        primary_button_properties(std::move(primary_button_properties)) {}

GlobalConfig& GlobalConfig::getGlobalConfig() {
        static GlobalConfig global_config{};
        static bool         parsed = false;

        if (!parsed) {
                ConfigMapper::mapToGlobalConfig(TomlParser::createConfig(), global_config);
                parsed = true;
        }

        return global_config;
}

const GlobalConfig& GlobalConfig::getDefaultGlobalConfig() {
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

        static GlobalConfig default_global_config = {std::move(default_window_properties),
                                                     std::move(default_primary_button_properties)};

        return default_global_config;
}

const WindowProperties& GlobalConfig::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& GlobalConfig::getPrimaryButtonProperties() const {
        return primary_button_properties;
}
