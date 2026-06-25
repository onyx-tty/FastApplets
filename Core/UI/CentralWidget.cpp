// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CentralWidget.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Widgets/PrimaryButton.h"

#include <algorithm>
#include <utility>
#include <vector>
#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QObject>
#include <QWidget>
#include <Qt>

namespace {

PrimaryButton* findPrimaryButton(int key, std::vector<PrimaryButton*> buttons) {
        const auto iter = std::find_if(buttons.cbegin(), buttons.cend(),
                                       [key](const PrimaryButton* button) -> bool {
                                               if (!button) { return false; }
                                               return button->getKeys().contains(key);
                                       });

        return iter != buttons.cend() ? *iter : nullptr;
}

bool isQuitKey(int key, const keybindings& quit_keys) {
        return quit_keys.contains(key);
}

} // namespace

CentralWidget::CentralWidget(std::vector<PrimaryButton*> buttons, const keybindings& quit_keys,
                             bool double_key_press, QWidget* parent) :
        QWidget(parent), buttons(std::move(buttons)), quit_keys(quit_keys),
        double_key_press(double_key_press) {
        setLayout(new QHBoxLayout(this));

        // Makes CentralWidget the parent of each button and adds them to the layout.
        for (auto* button : this->buttons) { layout()->addWidget(button); }
}

void CentralWidget::keyPressEvent(QKeyEvent* event) {
        int key = event->key();

        if (double_key_press) {
                // Quit pressed
                if (isQuitKey(key, quit_keys)) {
                        // Unselect if a button is focused
                        if (auto* focused = qobject_cast<PrimaryButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                                this->setFocus();
                        } else { // Quit if not
                                QApplication::quit();
                        }
                } else if (auto* primary_button = findPrimaryButton(key, buttons)) {
                        // Click if already focused
                        if (primary_button->hasFocus()) {
                                primary_button->animateClick();
                                primary_button->clearFocus();
                                this->setFocus();
                        } else { // Re-focus if not
                                if (auto* focused = qobject_cast<PrimaryButton*>(
                                            QApplication::focusWidget())) {
                                        focused->clearFocus();
                                }
                                primary_button->setFocus(Qt::FocusReason::MouseFocusReason);
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
        this->setFocus();
}

const std::vector<PrimaryButton*>& CentralWidget::getButtons() const {
        return buttons;
}
