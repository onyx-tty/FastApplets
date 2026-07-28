// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"
#include "Core/UI/Types/LayoutProperties.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <utility>

using namespace config::schema;

Config::Config(WindowParams window_params, PrimaryButtonStyle primary_button_style,
               LayoutProperties layout_properties) :
        window_params(std::move(window_params)),
        primary_button_style(std::move(primary_button_style)),
        layout_properties(std::move(layout_properties)) {}

const WindowParams& Config::getWindowParams() const {
        return window_params;
}

const PrimaryButtonStyle& Config::getPrimaryButtonStyle() const {
        return primary_button_style;
}

const LayoutProperties& Config::getLayoutProperties() const {
        return layout_properties;
}
