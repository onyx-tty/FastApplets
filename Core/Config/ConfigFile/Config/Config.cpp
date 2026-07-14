// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>

using namespace config::schema;
using namespace config::schema::properties;

Config::Config(Window window, PrimaryButton primary_button, Layout layout) :
        window_properties(std::move(window)), primary_button_properties(std::move(primary_button)),
        layout_properties(std::move(layout)) {}

const Window& Config::getWindowProperties() const {
        return window_properties;
}

const PrimaryButton& Config::getPrimaryButtonProperties() const {
        return primary_button_properties;
}

const Layout& Config::getLayoutProperties() const {
        return layout_properties;
}
