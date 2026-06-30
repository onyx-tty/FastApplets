// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <vector>

class PrimaryButton;
class PrimaryButtonProperties;
class QWidget;

// Constructs a vector of PrimaryButtons from passed PrimaryButtonParams, assigns
// visual properties from PrimaryButtonProperties, and keybindings from keys, with
// default_keys for fallback behavior.
class PrimaryButtonsFactory final {
public:
        PrimaryButtonsFactory() = delete;

        // Creates PrimaryButtons from given params, sets given properties and keys.
        //
        // Keybinding resolution order:
        // 1. User-configured keys (keys).
        // 2. Default keys (default_keys).
        // 3. Qt::Key_unknown if the defaults are exhausted.
        //
        // WARNING: If a resolved key is already bound elsewhere, both buttons
        //          will share it silently. A global keybinding validation pass is not
        //          implemented yet.
        //
        // Returns a vector containing every created button.
        // Calls qFatal instead if no button params are found.
        static std::vector<PrimaryButton*> create(const std::vector<PrimaryButtonParams>& params,
                                                  const PrimaryButtonProperties&  properties,
                                                  const std::vector<keybindings>& keys,
                                                  const std::vector<keybindings>& default_keys,
                                                  QWidget*                        parent);
};
