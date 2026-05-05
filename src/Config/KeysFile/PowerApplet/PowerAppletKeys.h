// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"

#include <vector>

class KeysMapper;

class PowerAppletKeys final : public GlobalKeys {
private:
        friend class KeysMapper;

        std::vector<keybindings> primary_button_keys;

public:
        explicit PowerAppletKeys(keybindings              quit_keys           = {},
                                 std::vector<keybindings> primary_button_keys = {});
        // TODO Make this const to avoid overwrites
        static PowerAppletKeys&         get();
        static const PowerAppletKeys&   getDefault();
        const std::vector<keybindings>& getPrimaryButtonKeys() const;
};
