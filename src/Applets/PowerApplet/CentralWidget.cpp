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
// TODO std::pair may not be necessary here and could potentially just be
//	overcomplicating the code
// TODO It might be possible to delegate some mechanisms here
//	to the buttons themselves
//	They should be streamlined if possible

CentralWidget::CentralWidget(QWidget* parent)
        : QWidget(parent) {
        main_layout = new QVBoxLayout(this);
        button_list = button::list(this, main_layout);
}


// TODO Consider QPointer
void CentralWidget::lastKeyUpdate(QKeyEvent* event) {
        if (last_key.first) {
                delete last_key.first;
        }
        last_key.first = event->clone();
        qInfo() << "event updated";
}

void CentralWidget::lastKeyUpdate(PowerButton* button) {
        last_key.second = button;
        qInfo() << "button updated";
}

void CentralWidget::lastKeyUpdate(QKeyEvent* event, PowerButton* button) {
        lastKeyUpdate(event);
        lastKeyUpdate(button);
}

void CentralWidget::keyPressEvent(QKeyEvent* event) { // match base keyPressEvent args
        keyPressEvent(event, nullptr); // forward to the modified keyPressEvent
        QWidget::keyPressEvent(event); // handle everything else as usual
}

void CentralWidget::keyPressEvent(QKeyEvent* event, PowerButton* button) {
        qInfo() << "INFO! keyPressEvent registered!";

        static bool in_num_range = false; // TODO Temporary variable, improve later
        for (unsigned i = 0; i <= 3; ++i) { // update button style
                if (event->key() == Qt::Key_1 + i) {
                        button_list[i]->setStyleSheet(style::selected);
                        button_list[i]->update();
                        button = button_list[i];
                        in_num_range = true;
                        qInfo() << "Enabling selection in" << button_list[i]->text();
                        break;
                }
                in_num_range = false;
        }

        // TODO Move to a separate keybindings file (inherited from DefaultKeybindings)
        std::array<unsigned, 4> power_keys = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4};
        if (event->key() == Keybinding::quit->key()) { // ESC pressed
                qInfo() << "esc pressed, quitting";
                QApplication::quit();
        // If nullptr or last key diff from current key
        } else if (!last_key.first || event->key() != last_key.first->key()) {
                qInfo() << "key and last_key don't match";
                selectButton(event, power_keys);
        // If last key matches current key
        } else {
                qInfo() << "key and last_key match";
                clickButton(event); // TODO Misleading, doesn't always result in a click, rework
        }

        if (last_key.second && last_key.second != button) { // remove last selection
                last_key.second->setStyleSheet(style::unselected);
                last_key.second->update();
                qInfo() << "Disabling selection in" << last_key.second->text();
        }

        if (in_num_range == false) {
                button = nullptr;
        }

        if (last_key.first || last_key.second) { // not nullptr
                qInfo() << "Current key combination:" << last_key.first->key()
                        << event->key();
        }
        lastKeyUpdate(event, button); // only update if button has already been processed
}

void CentralWidget::selectButton(QKeyEvent* event,
                                 std::array<unsigned, 4>& acceptable_keys) {
        for (unsigned i = 0; i <= 3; ++i) {
                // if in range
                if (event->key() == Qt::Key_1 + i) {
                        qInfo() << button_list[i]->text() << "selected!";

                        button_list[i]->setStyleSheet(style::selected);
                        button_list[i]->update();
                        return;
                }
        }
        // if null
        if (!last_key.second) {
                qWarning() << "INFO! last_key.second is null!";
        } else { // if not in range and last_key.second not nullptr
                qInfo() << "INFO! Key not within the range of acceptable_keys!";
                // unselect previous key
                last_key.second->setStyleSheet(style::unselected);
                last_key.second->update();
        }
}

void CentralWidget::clickButton(QKeyEvent* event) {
        qInfo() << "Current key combination: " << last_key.first->key() << event->key();
        for (unsigned i = 0; i <= 3; ++i) {
                if (event->key() == Qt::Key_1 + i) {
                        qInfo() << button_list[i]->text() << "clicked!";
                        last_key.second->setStyleSheet(style::unselected);
                        last_key.second->update();
                        emit button_list[i]->clicked();
                        QApplication::quit();
                }
        }
        qInfo() << "diff key";
}
