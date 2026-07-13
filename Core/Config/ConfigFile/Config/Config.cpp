// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>

config::schema::Config::Config(WindowProperties window, PrimaryButtonProperties primary_button,
                               LayoutProperties layout) :
        window_properties(std::move(window)), primary_button_properties(std::move(primary_button)),
        layout_properties(std::move(layout)) {}

const WindowProperties& config::schema::Config::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& config::schema::Config::getPrimaryButtonProperties() const {
        return primary_button_properties;
}

const LayoutProperties& config::schema::Config::getLayoutProperties() const {
        return layout_properties;
}
