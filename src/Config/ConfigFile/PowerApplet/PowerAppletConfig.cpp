// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletConfig.h"
#include "Applets/Types/AppletType.h"
#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/TomlParser/TomlParser.h"
#include "Config/Types/ConfigType.h"
#include "CppUtils/Log/QtLog.h"
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
                ConfigMapper::mapToPowerAppletConfig(TomlParser::parseFile(applet::type::power_applet,
                                                                           config::type::config),
                                                     TomlParser::parseFile(applet::type::global,
                                                                           config::type::config),
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
                {PowerButtonParams{power_button_type::shutdown, "Shutdown", 1, "systemctl shutdown",
                                   iconFor(power_button_type::shutdown)},
                 PowerButtonParams{power_button_type::reboot, "Reboot", 2, "systemctl reboot",
                                   iconFor(power_button_type::reboot)},
                 PowerButtonParams{power_button_type::suspend, "Suspend", 3, "systemctl suspend",
                                   iconFor(power_button_type::suspend)},
                 PowerButtonParams{power_button_type::hibernate, "Hibernate", 4,
                                   "systemctl hibernate", iconFor(power_button_type::hibernate)}};
        LayoutProperties default_layout_properties = LayoutProperties{std::move(primary_buttons)};

        static PowerAppletConfig default_power_applet_config =
                {std::move(default_window_properties), std::move(default_primary_button_properties),
                 std::move(default_layout_properties)};

        return default_power_applet_config;
}

const LayoutProperties& PowerAppletConfig::getLayoutProperties() const {
        return layout_properties;
}
