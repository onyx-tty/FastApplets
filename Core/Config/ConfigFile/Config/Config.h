// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Properties/Layout.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButton.h"
#include "Core/Config/ConfigFile/Properties/Window.h"

namespace config {
class ConfigMapper;
} // namespace config

namespace config::schema {

using properties::Layout;
using properties::PrimaryButton;
using properties::Window;

// Holds user-configured properties used by an applet.
class Config {
private:
        friend class config::ConfigMapper;

        Window        window_properties;
        PrimaryButton primary_button_properties;
        Layout        layout_properties;

public:
        explicit Config(Window window = Window{}, PrimaryButton primary_button = PrimaryButton{},
                        Layout layout = Layout{});
        [[nodiscard]] const Window&        getWindowProperties() const;
        [[nodiscard]] const PrimaryButton& getPrimaryButtonProperties() const;
        [[nodiscard]] const Layout&        getLayoutProperties() const;
};

} // namespace config::schema
