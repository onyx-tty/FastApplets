// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/WindowParams.h"

#include <utility>

using namespace config::schema;

Config::Config(WindowParams window_params, PrimaryButtonParams primary_button_params) :
        window_params(std::move(window_params)),
        primary_button_params(std::move(primary_button_params)) {}

const WindowParams& Config::getWindowParams() const {
        return window_params;
}

const PrimaryButtonParams& Config::getPrimaryButtonParams() const {
        return primary_button_params;
}
