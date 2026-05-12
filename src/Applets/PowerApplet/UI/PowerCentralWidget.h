// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "UI/Types/ButtonType.h"
#include "Widgets/PowerButton.h"

#include <vector>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QWidget>
#include <Qt>

class QShowEvent;

// Keyboard navigation follows a two-press "stage-then-confirm" pattern to
// prevent accidental triggers of destructive actions (shutdown, reboot, etc.):
//
//   First press -> focuses ("stages") the matching button; visible via highlights.
//   Second pres -> animates and triggers the click, then clears focus.
//   Quit key    -> clears focus if a button is staged, otherwise quits the app.
//
// Key:Button mapping is loaded from keys.toml via createButtons().
// Default mapping: Key_1->button 1, Key_2->button 2, and so on.
//
// BUG: Mouse clicks trigger the sunken visual effect that can only be disabled
//      by pressing the quit key.
class PowerCentralWidget final : public QWidget {
        Q_OBJECT

private:
        // Creates and registers all PowerButtons from Config and Keys.
        //
        // Reads buttons and definitions from PowerAppletConfig and keybindings
        // from PowerAppletKeys, constructs a PowerButton widget for each entry,
        // adds it to main_layout, and returns a vector with every button found.
        //
        // WARNING: If a resolved key is already bound elsewhere, both buttons
        // will share it silently. A global keybinding validation pass is not
        // implemented yet.
        //
        // Keybinding resolution order:
        // 1. User-configured keys (PowerAppletKeys::get()).
        // 2. Default keys (PowerAppletKeys::getDefault()).
        // 3. Qt::Key_unknown if the defaults are exhausted.
        //
        // Returns a vector containing every created button.
        // Calls QFATAL if no buttons are found in config.
        std::vector<PowerButton*> createButtons();

        // TODO: Don't store main_layout separately, QWidget already stores the layout.
        //       This change should be applied to all classes that do it.
        QBoxLayout*               main_layout;
        std::vector<PowerButton*> buttons;

public:
        explicit PowerCentralWidget();
        const QBoxLayout*                getMainLayout() const;
        const std::vector<PowerButton*>& getButtons() const;
        // Adds the stage-then-confirm keyboard navigation. See class doc for more.
        void                             keyPressEvent(QKeyEvent* event) override;
        // Clears button focus on show so no button starts pre-staged, keeping the
        // stage-then-confirm flow consistent from the first keypress.
        void                             showEvent(QShowEvent* event) override;
};
