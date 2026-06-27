// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/ConfigFile/Global/GlobalConfig.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

class ConfigMapper;

// Config for PowerApplet, loaded from config.toml.
class ActionAppletConfig final : public GlobalConfig {
private:
        friend class ConfigMapper;

        LayoutProperties layout_properties;

public:
        ActionAppletConfig(WindowProperties        window         = WindowProperties{},
                           PrimaryButtonProperties primary_button = PrimaryButtonProperties{},
                           LayoutProperties        layout         = LayoutProperties{});
        [[nodiscard]] const LayoutProperties& getLayoutProperties() const;
};
