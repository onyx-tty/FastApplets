// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

class ConfigMapper;

namespace config::schema {

using properties::Layout;
using properties::PrimaryButton;
using properties::Window;

// Holds user-configured properties used by an applet.
class Config {
private:
        friend class ::ConfigMapper;

        Window        window_properties;
        PrimaryButton primary_button_properties;
        Layout        layout_properties;

public:
        explicit Config(Window window = Window{}, PrimaryButton primary_button = PrimaryButton{},
                        Layout layout = Layout{});
        const Window&        getWindowProperties() const;
        const PrimaryButton& getPrimaryButtonProperties() const;
        const Layout&        getLayoutProperties() const;
};

} // namespace config::schema
