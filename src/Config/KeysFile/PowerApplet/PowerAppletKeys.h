// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"

#include <vector>

class KeysMapper;

// Keys for PowerApplet, loaded from keys.toml.
//
// Two pre-installed singletons are accessible via static methods:
//   get()        - user-defined settings from keys.toml
//   getDefault() - hardcoded fallbacks for missing or invalid keybindings
//
// TODO: The singleton instances and the keys schema are coupled in the same
//       class. They should be separated so the data structure can exist
//       independently of its accessor. Currently held together with duct tape.
class PowerAppletKeys final : public GlobalKeys {
private:
        friend class KeysMapper;

        std::vector<keybindings> primary_button_keys;

public:
        explicit PowerAppletKeys(keybindings              quit_keys           = {},
                                 std::vector<keybindings> primary_button_keys = {});
        static PowerAppletKeys&         get();
        static const PowerAppletKeys&   getDefault();
        const std::vector<keybindings>& getPrimaryButtonKeys() const;
};
