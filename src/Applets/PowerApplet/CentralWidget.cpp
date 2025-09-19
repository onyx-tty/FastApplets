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
#include "../../Config/PowerKeybindings.h"
#include "../../Config/PowerLayout.h"

#include <QApplication>
#include <QDebug>
#include <QFocusEvent>

#include <array>

// TODO std::pair may not be necessary here and could potentially just be
//	overcomplicating the code
// TODO It might be possible to delegate some mechanisms here
//	to the buttons themselves
//	They should be streamlined if possible

CentralWidget::CentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)) {
        last_key    = std::pair<QKeyEvent*, PowerButton*>(nullptr, nullptr); // TODO Clean this up
        layout.layout_prop.initButtonList(this, main_layout, layout.layout_prop);
        button_list = layout.layout_prop.getButtonList();
}

void CentralWidget::keyPressEvent(QKeyEvent* event) {
        qInfo() << "INFO! keyPressEvent registered!";

        auto inPowerRange = [](QKeyEvent* event) {
                int diff = Qt::Key_4 - event->key();
                return diff >= 0 && diff <= 4;
        };

        // update button style
        updatePowerButton(event, layout.style_prop.selected, [&](int i) { ; });

        if (event->key() == keybindings.quit->key() && last_key.first
            && inPowerRange(last_key.first)) { // ESC pressed with focus, unselect last key
                updatePowerButton(last_key.first, layout.style_prop.unselected,
                                  [&](int i) { updateLastKey(event, nullptr); });
        } else if (event->key() == keybindings.quit->key()
                   && !inPowerRange(event)) { // ESC pressed without focus, quit
                qInfo() << "ESC pressed, quitting!";
                QApplication::quit();
        } else if (!last_key.first || event->key() != last_key.first->key()) { // select
                qInfo() << "event and last_key.first don't match";
                // select current
                updatePowerButton(event, layout.style_prop.selected, [&](int i) {
                        qInfo() << button_list[i]->text() << "selected!";
                });
                // unselect previous
                if (!last_key.second) {
                        qWarning() << "INFO! last_key.second is null!";
                } else if (!inPowerRange(event)) {
                        qInfo() << "INFO! Key not within the range of power keys!";
                } else {
                        setPowerButtonStyle(last_key.second, layout.style_prop.unselected, event);
                }
        } else { // click
                qInfo() << "event and last_key.first match";
                updatePowerButton(last_key.first, layout.style_prop.unselected, [&](int i) {
                        emit button_list[i]->clicked();
                        QApplication::quit();
                });
                qInfo() << "diff key";
        }

        qInfo() << "Current key combination:"
                << (last_key.first ? QString::number(last_key.first->key()) : "NULL")
                << QString::number(event->key());

        for (int i = 0; i != 4; ++i) { // only update when the button has already been processed
                if (event->key() == keybindings.power_keys[i]) updateLastKey(event, button_list[i]);
        }
}

// TODO Consider QPointer
void CentralWidget::updateLastKey(QKeyEvent* event, PowerButton* button) {
        if (last_key.first) { delete last_key.first; }
        last_key.first = event->clone();
        last_key.second = button;
}

void CentralWidget::setPowerButtonStyle(PowerButton* button, const QString style, QKeyEvent* event) {
        button->setStyleSheet(style);
        button->update();
}

void CentralWidget::updatePowerButton(QKeyEvent* event, const QString style, auto&& action) {
        for (int i = 0; i != 4; ++i) {
                if (event->key() == keybindings.power_keys[i]) {
                        setPowerButtonStyle(button_list[i], style, event);
                        action(i); // call lambda and let it access i
                        return;
                }
        }
}
