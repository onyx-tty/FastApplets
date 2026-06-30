// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

class ConfigMapper;

// Holds user-configured properties used by an applet.
class Config {
private:
        friend class ConfigMapper;

        WindowProperties        window_properties;
        PrimaryButtonProperties primary_button_properties;
        LayoutProperties        layout_properties;

public:
        explicit Config(WindowProperties        window         = WindowProperties{},
                        PrimaryButtonProperties primary_button = PrimaryButtonProperties{},
                        LayoutProperties        layout         = LayoutProperties{});
        const WindowProperties&        getWindowProperties() const;
        const PrimaryButtonProperties& getPrimaryButtonProperties() const;
        const LayoutProperties&        getLayoutProperties() const;
};
