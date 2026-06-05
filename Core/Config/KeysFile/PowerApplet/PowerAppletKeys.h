// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Global/GlobalKeys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>

class KeysMapper;

// Keys for PowerApplet, loaded from keys.toml.
class PowerAppletKeys final : public GlobalKeys {
private:
        friend class KeysMapper;

        // TODO: Rename to primary_buttons
        std::vector<keybindings> primary_button;

public:
        explicit PowerAppletKeys(keybindings              quit           = {},
                                 std::vector<keybindings> primary_button = {});
        const std::vector<keybindings>& getPrimaryButton() const;
};
