// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GlobalConfig.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>

GlobalConfig::GlobalConfig(WindowProperties window, PrimaryButtonProperties primary_button) :
        window_properties(std::move(window)), primary_button_properties(std::move(primary_button)) {
}

const WindowProperties& GlobalConfig::getWindowProperties() const {
        return window_properties;
}

const PrimaryButtonProperties& GlobalConfig::getPrimaryButtonProperties() const {
        return primary_button_properties;
}
