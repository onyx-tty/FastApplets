// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Keybindings.h"

#include <vector>

namespace config::schema {

struct Keys final {
        keybindings              quit;
        std::vector<keybindings> primary_buttons;
};

} // namespace config::schema
