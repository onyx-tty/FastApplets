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
// PrimaryButtonStyle, and assigns keybindings from keys.
//
// If a key is missing, a warning is logged and a keybindings set consisting of Qt::Keys_unknown
// is returned.
//
// WARNING: If a resolved key is already bound elsewhere, both buttons
//          will share it silently. A global keybinding validation pass is not
//          implemented yet.
//
// Returns a PrimaryButtons vector containing every created button.
// Calls qFatal instead if no button params are found.
PrimaryButtons makePrimaryButtons(const PrimaryButtonParams& params,
        const std::vector<keybindings>& keys_vec, QWidget* parent);
