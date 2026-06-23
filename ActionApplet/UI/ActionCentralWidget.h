// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/CentralWidget.h"

#include <vector>
#include <QObject>
#include <Qt>

class ActionAppletConfig;
class ActionAppletKeys;
class ActionButton;
class PrimaryButton;
class QKeyEvent;
class QShowEvent;
class QWidget;

class ActionCentralWidget final : public CentralWidget {
        Q_OBJECT

private:
        // Creates and registers all ActionButtons from Config and Keys.
        //
        // Reads buttons and definitions from config and keybindings from
        // ActionAppletKeys, constructs a ActionButton widget for each entry,
        // adds it to the layout, and returns a vector with every button found.
        //
        // WARNING: If a resolved key is already bound elsewhere, both buttons
        // will share it silently. A global keybinding validation pass is not
        // implemented yet.
        //
        // Keybinding resolution order:
        // 1. User-configured keys (keys).
        // 2. Default keys (default_keys).
        // 3. Qt::Key_unknown if the defaults are exhausted.
        //
        // Returns a vector containing every created button.
        // Calls qFatal if no buttons are found in config.
        [[nodiscard]] std::vector<PrimaryButton*> createButtons(
                const ActionAppletConfig& config, const ActionAppletKeys& keys,
                const ActionAppletKeys& default_keys);

        std::vector<ActionButton*> buttons;

public:
        explicit ActionCentralWidget(const ActionAppletConfig& config, const ActionAppletKeys& keys,
                                     const ActionAppletKeys& default_keys, QWidget* parent);
        [[nodiscard]] const std::vector<ActionButton*>& getButtons() const;
};
