// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

// Config for PowerApplet, loaded from config.toml.
//
// Two pre-installed singletons are accessible via static methods:
//   get()        - user-defined settings from config.toml
//   getDefault() - hardcoded fallbacks for missing or invalid settings
//
// TODO: The singleton instances and the config schema are coupled in the same
//       class. They should be separated so the data structure can exist
//       independently of its accessor. Currently held together with duct tape.
class PowerAppletConfig final : public GlobalConfig {
private:
        friend class ConfigMapper;

        LayoutProperties layout_properties;

public:
        PowerAppletConfig(
                WindowProperties        window_properties         = WindowProperties{},
                PrimaryButtonProperties primary_button_properties = PrimaryButtonProperties{},
                LayoutProperties        layout_properties         = LayoutProperties{});
        static PowerAppletConfig&       get();
        static const PowerAppletConfig& getDefault();
        const LayoutProperties&         getLayoutProperties() const;
};
