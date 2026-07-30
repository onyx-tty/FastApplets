// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>

namespace config {
class KeysMapper;
} // namespace config

namespace config::schema {

// Holds user-configured keys used by an applet.
class Keys {
private:
        friend class config::KeysMapper;

        keybindings              quit;
        std::vector<keybindings> primary_buttons;

public:
        explicit Keys(keybindings quit = {}, std::vector<keybindings> primary_buttons = {});
        [[nodiscard]] const keybindings&              getQuit() const;
        [[nodiscard]] const std::vector<keybindings>& getPrimaryButtons() const;
};

} // namespace config::schema
