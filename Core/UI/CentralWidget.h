// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>
#include <QObject>
#include <QWidget>

class PrimaryButton;
class QKeyEvent;

// By default, keyboard navigation follows a two-press "stage-then-confirm" pattern to
// prevent accidental triggers of destructive actions (shutdown, reboot, etc.):
//
//   First press -> focuses ("stages") the matching button; visible via highlights.
//   Second pres -> animates and triggers the click, then clears focus.
//   Quit key    -> clears focus if a button is staged, otherwise quits the app.
//
// 'primary_button.double_key_press = false' in config.toml disables this, making
// each key press instantly trigger button click.
//
// Key:Button mapping is loaded from keys.toml via createButtons().
// Default mapping: Key_1->button 1, Key_2->button 2, and so on.
//
// BUG: Mouse clicks trigger the sunken visual effect that can only be disabled
//      by pressing the quit key.
class CentralWidget : public QWidget {
        Q_OBJECT

protected:
        explicit CentralWidget(std::vector<PrimaryButton*> buttons, const keybindings& quit_keys,
                               bool double_key_press, QWidget* parent);
        virtual ~CentralWidget() = 0;
        // Adds the stage-then-confirm keyboard navigation. See class doc for more.
        void keyPressEvent(QKeyEvent* event) override;
        // Clears button focus on show so no button starts pre-staged, keeping the
        // stage-then-confirm flow consistent from the first keypress.
        void showEvent(QShowEvent* event) override;

        std::vector<PrimaryButton*> buttons;
        const keybindings& quit_keys;
        bool               double_key_press;

public:
        [[nodiscard]] const std::vector<PrimaryButton*>& getButtons() const;
};
