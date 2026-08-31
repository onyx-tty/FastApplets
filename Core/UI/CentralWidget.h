// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Keybindings.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Widgets/PrimaryButtons.h"

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
// TODO: Add staging for mouse
class CentralWidget : public QWidget {
        Q_OBJECT

private:
        PrimaryButtons buttons;
        keybindings    quit_keys;
        bool           double_key_press;

        void handleQuitKey();
        void handlePrimaryButtonKey(int key, bool double_key_press);

public:
        explicit CentralWidget(PrimaryButtons buttons, keybindings quit_keys,
                PrimaryButtonBehavior behavior, QWidget* parent);

        // Adds the stage-then-confirm keyboard navigation. See class doc for more.
        void keyPressEvent(QKeyEvent* event) override;

        // Clears button focus on show so no button starts pre-staged, keeping the
        // stage-then-confirm flow consistent from the first keypress.
        void showEvent(QShowEvent* event) override;

        [[nodiscard]] const PrimaryButtons& getButtons() const;
};
