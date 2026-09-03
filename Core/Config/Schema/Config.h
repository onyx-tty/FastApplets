// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Shell/Types/ShellContext.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/WindowParams.h"

namespace config::schema {

struct Config final {
        ShellContext        shell_context;
        WindowParams        window_params;
        PrimaryButtonParams primary_button_params;
};

} // namespace config::schema
