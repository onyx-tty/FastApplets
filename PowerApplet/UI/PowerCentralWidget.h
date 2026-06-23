// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/CentralWidget.h"

#include <vector>
#include <QObject>
#include <Qt>

class PowerAppletConfig;
class PowerAppletKeys;
class PowerButton;
class PrimaryButton;
class QKeyEvent;
class QShowEvent;
class QWidget;

class PowerCentralWidget final : public CentralWidget {
        Q_OBJECT

private:
        // Creates and registers all PowerButtons from Config and Keys.
        //
        // Reads buttons and definitions from config and keybindings from
        // PowerAppletKeys, constructs a PowerButton widget for each entry,
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
        std::vector<PrimaryButton*> createButtons(const PowerAppletConfig& config,
                                                  const PowerAppletKeys&   keys,
                                                  const PowerAppletKeys&   default_keys);

        std::vector<PowerButton*> buttons;

public:
        explicit PowerCentralWidget(const PowerAppletConfig& config, const PowerAppletKeys& keys,
                                    const PowerAppletKeys& default_keys, QWidget* parent);
        [[nodiscard]] const std::vector<PowerButton*>& getButtons() const;
};
