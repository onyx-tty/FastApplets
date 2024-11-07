/* Pretty Applets

   Copyright (C) 2024 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "CentralWidget.h"
#include "../../Config/SharedKeybindings.h" // TODO Inherited keybindings
#include "../../Config/PowerLayout.h"

#include <QApplication>
#include <QFocusEvent>
#include <QDebug>

#include <array>

// TODO Clean up syntax for consistency

CentralWidget::CentralWidget(QWidget* parent)
        : QWidget(parent) {
        main_layout = new QVBoxLayout(this);
        button_list = button::list(this, main_layout);
}

void CentralWidget::keyPressEvent(QKeyEvent* event) { // match base keyPressEvent args
        keyPressEvent(event, nullptr); // forward to the modified keyPressEvent
        QWidget::keyPressEvent(event); // handle everything else as usual
}

void CentralWidget::lastKeyUpdateEvent(QKeyEvent* event) {
        delete last_key.first;
        last_key.first = event->clone();
}

void CentralWidget::keyPressEvent(QKeyEvent* event, PowerButton* button) {
        qInfo() << "INFO! keyPressEvent registered!";

        auto clear_key = [](std::pair<QKeyEvent*, PowerButton*> key) {
                key.first = nullptr;
                key.second = nullptr;
        };

        // TODO Move to a separate keybindings file (inherited from DefaultKeybindings)
        std::array<unsigned, 4> power_keys = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4};
        if (event->key() == Keybinding::quit->key()) { // ESC pressed
                qInfo() << "esc pressed, quitting";
                QApplication::quit();
        } else if (last_key.first == nullptr && button == nullptr) { // last_key empty
                qInfo() << "last_key was <nullptr, nullptr>, initializing";
                lastKeyUpdateEvent(event);
                last_key.second = button;
        } else if (event->key() == last_key.first->key()) { // key and last_key match
                qInfo() << "key and last_key match";
                // pressed either 1, 2, 3 or 4
                clickButton(event); // TODO Misleading, doesn't always result in a click, rework
                clear_key(last_key);
                lastKeyUpdateEvent(event);
        } else {
                qInfo() << "key and last_key don't match";
                selectButton(event, button, power_keys);
                clear_key(last_key);
                lastKeyUpdateEvent(event);
        }
}

void CentralWidget::selectButton(QKeyEvent* event,
                                 PowerButton* button,
                                 std::array<unsigned, 4>& acceptable_keys) {
        qInfo() << "Current combo: " << last_key.first->key() << event->key();
        QFocusEvent selection(QEvent::FocusIn, Qt::OtherFocusReason);

        for (unsigned i = 0; i <= 3; ++i) {
                if (event->key() == Qt::Key_1 + i) {
                        qInfo() << button_list[i]->text() << "selected!";
                        last_key.second = button_list[i];
                        emit button_list[i]->focusInEvent(&selection);
                        return;
                }
        }

        if (!last_key.second) {
                qWarning() << "WARNING! last_key.second is null, cannot emit safely!";
                return;
        } else {
                qInfo() << "INFO! Keys not within the range of acceptable_keys!";
        }
        QFocusEvent selection_dropped(QEvent::FocusOut, Qt::OtherFocusReason);

        emit last_key.second->focusOutEvent(&selection_dropped);
}

void CentralWidget::clickButton(QKeyEvent* event) {
        qInfo() << "Current combo: " << last_key.first->key() << event->key();
        for (unsigned i = 0; i <= 3; ++i) {
                if (event->key() == Qt::Key_1 + i) {
                        qInfo() << button_list[i]->text() << "clicked!";
                        emit button_list[i]->clicked();
                        return;
                }
        }
        qInfo() << "diff key";
}
