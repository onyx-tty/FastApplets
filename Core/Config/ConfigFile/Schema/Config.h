// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/WindowParams.h"

namespace config {
class ConfigMapper;
} // namespace config

namespace config::schema {

// Holds user configuration used by the applets.
class Config {
private:
        friend class config::ConfigMapper;

        WindowParams        window_params;
        PrimaryButtonParams primary_button_params;

public:
        explicit Config(WindowParams        window_params         = WindowParams(),
                        PrimaryButtonParams primary_button_params = PrimaryButtonParams());
        [[nodiscard]] const WindowParams&        getWindowParams() const;
        [[nodiscard]] const PrimaryButtonParams& getPrimaryButtonParams() const;
};

} // namespace config::schema
