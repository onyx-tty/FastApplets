// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>

Config::Config(WindowProperties window, PrimaryButtonProperties primary_button,
               LayoutProperties layout) :
        window_properties(std::move(window)), primary_button_properties(std::move(primary_button)),
        layout_properties(std::move(layout)) {}

const WindowProperties& Config::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& Config::getPrimaryButtonProperties() const {
        return primary_button_properties;
}

const LayoutProperties& Config::getLayoutProperties() const {
        return layout_properties;
}
