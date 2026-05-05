// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

class PowerAppletConfig final : public GlobalConfig {
private:
        friend class ConfigMapper;

        LayoutProperties      layout_properties;
        EnvironmentProperties environment_properties;

public:
        PowerAppletConfig(
                WindowProperties        window_properties         = WindowProperties{},
                PrimaryButtonProperties primary_button_properties = PrimaryButtonProperties{},
                LayoutProperties        layout_properties         = LayoutProperties{},
                EnvironmentProperties   environment_properties    = EnvironmentProperties{});
        // TODO Make this const to avoid overwrites
        static PowerAppletConfig&       get();
        static const PowerAppletConfig& getDefault();
        const LayoutProperties&         getLayoutProperties() const;
        const EnvironmentProperties&    getEnvironmentProperties() const;
};
