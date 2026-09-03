// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CentralWidget.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Widgets/PrimaryButton.h"
#include "Core/UI/Widgets/PrimaryButtons.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QObject>
#include <QWidget>
#include <Qt>
#include <utility>

namespace {

[[nodiscard]] bool isQuitKey(int key, const keybindings& quit_keys) {
        return quit_keys.contains(key);
}

} // namespace

void CentralWidget::handleQuitKey() {
        auto* focused_button = qobject_cast<PrimaryButton*>(QApplication::focusWidget());

        // If no button is focused then quit should simply terminate the program.
        if (!focused_button) {
                QApplication::quit();
                return;
        }

        // Otherwise it resets focus.
        focused_button->clearFocus();
        this->setFocus(Qt::FocusReason::OtherFocusReason);
}

void CentralWidget::handlePrimaryButtonKey(int key, bool double_key_press) {
        // Find the primary button with that key.
        auto* primary_button = findPrimaryButton(key, buttons);

        // Click it if it's focused or if double_key_press=true, which disables a two-press
        // stage-then-press mechanism and makes buttons instantly clickable.
        if (primary_button->hasFocus() || !double_key_press) {
                primary_button->animateClick();
        } else { // Otherwise focus it.
                if (auto* focused = qobject_cast<PrimaryButton*>(QApplication::focusWidget())) {
                        focused->clearFocus();
                }
                primary_button->setFocus(Qt::FocusReason::OtherFocusReason);
        }
}

CentralWidget::CentralWidget(PrimaryButtons buttons, keybindings quit_keys,
        PrimaryButtonBehavior behavior, QWidget* parent) :
        QWidget(parent), buttons(std::move(buttons)), quit_keys(std::move(quit_keys)),
        double_key_press(behavior.double_key_press) {
        setLayout(new QHBoxLayout(this));

        // CentralWidget should be made the parent of each button to prevent memory leaks.
        for (auto* button : this->buttons) { layout()->addWidget(button); }
}

void CentralWidget::keyPressEvent(QKeyEvent* event) {
        int key = event->key();

        if (isQuitKey(key, quit_keys)) {
                handleQuitKey();
        } else if (auto* primary_button = findPrimaryButton(key, buttons)) {
                handlePrimaryButtonKey(key, double_key_press);
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
