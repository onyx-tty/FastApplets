// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "UI/Types/ButtonID.h"
#include "Widgets/PowerButton.h"
#include "Widgets/PowerButtonRecord.h"

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
        PowerButtonRecords createButtons();

        QBoxLayout*        main_layout;
        PowerButtonRecords buttons;

public:
        explicit PowerCentralWidget();
        const QBoxLayout*         getMainLayout() const;
        const PowerButtonRecords& getButtons() const;
        void                      keyPressEvent(QKeyEvent* event) override;
        void                      showEvent(QShowEvent* event) override;
};
