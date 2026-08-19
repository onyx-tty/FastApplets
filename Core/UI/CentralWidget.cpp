// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CentralWidget.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtons.h"
#include "Widgets/PrimaryButton.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QObject>
#include <QWidget>
#include <Qt>
#include <algorithm>
#include <utility>

namespace {

[[nodiscard]] PrimaryButton* findPrimaryButton(int key, PrimaryButtons buttons) {
        const auto iter = std::find_if(
                buttons.cbegin(), buttons.cend(), [key](const PrimaryButton* button) -> bool {
                        if (!button) { return false; }
                        return button->getKeys().contains(key);
                });

        return iter != buttons.cend() ? *iter : nullptr;
}

[[nodiscard]] bool isQuitKey(int key, const keybindings& quit_keys) {
        return quit_keys.contains(key);
}

} // namespace

CentralWidget::CentralWidget(PrimaryButtons buttons, keybindings quit_keys,
        PrimaryButtonBehavior behavior, QWidget* parent) :
        QWidget(parent), buttons(std::move(buttons)), quit_keys(std::move(quit_keys)),
        double_key_press(behavior.double_key_press) {
        setLayout(new QHBoxLayout(this));

        // Makes CentralWidget the parent of each button and adds them to the layout.
        for (auto* button : this->buttons) { layout()->addWidget(button); }
}

// TODO: Simplify this function by overriding each event signal or by hooking
//       actions to appropriate signals.
//       As an example, focus is already set to parent when clicked, it does not
//       have to be done again in here.
// TODO: Make this function easier to read by creating helpers for some of the
//       complex actions, such as changing focus from one button to another,
//       unselecting if a button is already focused, and more.
void CentralWidget::keyPressEvent(QKeyEvent* event) {
        int key = event->key();

        if (double_key_press) {
                // Quit pressed
                if (isQuitKey(key, quit_keys)) {
                        // Unselect if a button is focused
                        if (auto* focused = qobject_cast<PrimaryButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                                this->setFocus(Qt::FocusReason::OtherFocusReason);
                        } else { // Quit if not
                                QApplication::quit();
                        }
                } else if (auto* primary_button = findPrimaryButton(key, buttons)) {
                        // Click if already focused
                        if (primary_button->hasFocus()) {
                                primary_button->animateClick();
                                primary_button->clearFocus();
                                this->setFocus(Qt::FocusReason::OtherFocusReason);
                        } else { // Re-focus if not
                                if (auto* focused = qobject_cast<PrimaryButton*>(
                                            QApplication::focusWidget())) {
                                        focused->clearFocus();
                                }
                                primary_button->setFocus(Qt::FocusReason::OtherFocusReason);
                        }
                }
        } else {
                if (isQuitKey(key, quit_keys)) {
                        QApplication::quit();
                } else if (auto* primary_button = findPrimaryButton(key, buttons)) {
                        primary_button->animateClick();
                }
        }
}

void CentralWidget::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);

        if (auto* focused = qobject_cast<PrimaryButton*>(QApplication::focusWidget())) {
                focused->clearFocus();
        }
        this->setFocus(Qt::FocusReason::OtherFocusReason);
}

const PrimaryButtons& CentralWidget::getButtons() const { return buttons; }
