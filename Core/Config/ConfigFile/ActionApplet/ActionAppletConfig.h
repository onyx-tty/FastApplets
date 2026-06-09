// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/ConfigFile/Global/GlobalConfig.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/UI/Widgets/ActionButtonParams.h"

class ConfigMapper;

// Config for PowerApplet, loaded from config.toml.
class ActionAppletConfig final : public GlobalConfig {
private:
        friend class ConfigMapper;

        LayoutProperties<ActionButtonParams> layout_properties;

public:
        ActionAppletConfig(WindowProperties        window         = WindowProperties{},
                           PrimaryButtonProperties primary_button = PrimaryButtonProperties{},
                           LayoutProperties<ActionButtonParams> layout =
                                   LayoutProperties<ActionButtonParams>{});
        [[nodiscard]] const LayoutProperties<ActionButtonParams>& getLayoutProperties() const;
};
