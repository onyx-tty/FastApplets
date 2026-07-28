// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"
#include "Core/UI/Types/LayoutProperties.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <utility>

using namespace config::schema;

Config::Config(WindowParams window_params, PrimaryButtonStyle primary_button_style,
               PrimaryButtonBehavior primary_button_behavior, LayoutProperties layout_properties) :
        window_params(std::move(window_params)),
        primary_button_style(std::move(primary_button_style)),
        primary_button_behavior(primary_button_behavior),
        layout_properties(std::move(layout_properties)) {}

const WindowParams& Config::getWindowParams() const {
        return window_params;
}

const PrimaryButtonStyle& Config::getPrimaryButtonStyle() const {
        return primary_button_style;
}

const PrimaryButtonBehavior& Config::getPrimaryButtonBehavior() const {
        return primary_button_behavior;
}

const LayoutProperties& Config::getLayoutProperties() const {
        return layout_properties;
}
