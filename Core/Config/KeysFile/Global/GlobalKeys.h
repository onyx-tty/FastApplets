// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"

// Base keys holding properties common to all applets.
//
// Derive from this and extend with applet-specific keybindings.
class GlobalKeys {
protected:
        explicit GlobalKeys(keybindings quit);

        keybindings quit;

public:
        const keybindings& getQuit() const;
};
