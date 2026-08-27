// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Keybindings.h"
#include "Core/UI/Types/PrimaryButtonParams.h"

#include <QWidget>
#include <vector>

class PrimaryButton;

using PrimaryButtons = std::vector<PrimaryButton*>;

// Looks for the first button in 'buttons' whose keybindings contain 'key'.
// If no button is found, returns nullptr instead.
[[nodiscard]] PrimaryButton* findPrimaryButton(int key, PrimaryButtons buttons);

// Constructs PrimaryButtons from given PrimaryButtonParams, assigns visual properties from
// PrimaryButtonStyle, and keybindings from keys, with fallback default_keys
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
// TODO: This function does too much. It should not resolve keys on top of button construction.
PrimaryButtons makePrimaryButtons(const PrimaryButtonParams& params,
        const std::vector<keybindings>& keys, const std::vector<keybindings>& default_keys,
        QWidget* parent);
