// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/WindowParams.h"

namespace config::schema {

// Holds user configuration used by the applets.
struct Config final {
        WindowParams        window_params;
        PrimaryButtonParams primary_button_params;
};

} // namespace config::schema
