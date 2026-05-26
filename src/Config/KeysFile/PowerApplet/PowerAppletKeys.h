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

        // TODO: Rename to primary_buttons
        std::vector<keybindings> primary_button;

public:
        explicit PowerAppletKeys(keybindings              quit           = {},
                                 std::vector<keybindings> primary_button = {});
        static const PowerAppletKeys&   get();
        static const PowerAppletKeys&   getDefault();
        const std::vector<keybindings>& getPrimaryButton() const;
};
