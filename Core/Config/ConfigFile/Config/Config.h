// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/LayoutProperties.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

namespace config {
class ConfigMapper;
} // namespace config

namespace config::schema {

// Holds user configuration used by the applets.
class Config {
private:
        friend class config::ConfigMapper;

        WindowParams       window_params;
        PrimaryButtonStyle primary_button_style;
        LayoutProperties   layout_properties;

public:
        explicit Config(WindowParams       window_params        = WindowParams(),
                        PrimaryButtonStyle primary_button_style = PrimaryButtonStyle(),
                        LayoutProperties   layout_properties    = LayoutProperties());
        [[nodiscard]] const WindowParams&       getWindowParams() const;
        [[nodiscard]] const PrimaryButtonStyle& getPrimaryButtonStyle() const;
        [[nodiscard]] const LayoutProperties&   getLayoutProperties() const;
};

} // namespace config::schema
