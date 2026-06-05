// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/ConfigFile/Global/GlobalConfig.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/UI/Widgets/PowerButtonParams.h"

class ConfigMapper;

// Config for PowerApplet, loaded from config.toml.
class PowerAppletConfig final : public GlobalConfig {
private:
        friend class ConfigMapper;

        LayoutProperties<PowerButtonParams> layout_properties;

public:
        PowerAppletConfig(
                WindowProperties                    window         = WindowProperties{},
                PrimaryButtonProperties             primary_button = PrimaryButtonProperties{},
                LayoutProperties<PowerButtonParams> layout = LayoutProperties<PowerButtonParams>{});
        const LayoutProperties<PowerButtonParams>& getLayoutProperties() const;
};
