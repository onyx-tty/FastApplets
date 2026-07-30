// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>

namespace config {
class KeysMapper;
} // namespace config

namespace config::schema {

// Holds user-configured keys used by an applet.
struct Keys final {
        keybindings              quit;
        std::vector<keybindings> primary_buttons;
};

} // namespace config::schema
