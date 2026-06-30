// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>

class KeysMapper;

// Holds user-configured keys used by an applet.
class Keys {
private:
        friend class KeysMapper;

        keybindings              quit;
        std::vector<keybindings> primary_buttons;

public:
        explicit Keys(keybindings quit = {}, std::vector<keybindings> primary_buttons = {});
        const keybindings&              getQuit() const;
        const std::vector<keybindings>& getPrimaryButtons() const;
};
