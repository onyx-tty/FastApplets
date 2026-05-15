// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GlobalConfig.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>

GlobalConfig::GlobalConfig(WindowProperties        window_properties,
                           PrimaryButtonProperties primary_button_properties) :
        window_properties(std::move(window_properties)),
        primary_button_properties(std::move(primary_button_properties)) {}

const WindowProperties& GlobalConfig::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& GlobalConfig::getPrimaryButtonProperties() const {
        return primary_button_properties;
}
