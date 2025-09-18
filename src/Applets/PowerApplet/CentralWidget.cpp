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
#include "../../Config/PowerLayout.h"

#include <QApplication>
#include <QDebug>
#include <QFocusEvent>

// prevent external linkage via static
static const std::array<PowerButton*, 4>* initButtonList(const QApplication& app, QBoxLayout* main_layout) {
        // We give PowerLayoutManager access to runtime and central widget's main layout
        // to be able to relatively locate .env and create a list of buttons within our layout
        PowerLayoutManager::setup(app, main_layout);
        return PowerLayoutManager::getLayoutProp().getButtonList();
}

Action::Action(int key, PowerButton* button) : key(key), button(button) {}

CentralWidget::CentralWidget(QWidget* parent, const QApplication& app) :
        QWidget(parent), main_layout(new QHBoxLayout(this)), last_action(Qt::Key_unknown, nullptr),
        current_action(Qt::Key_unknown, nullptr), button_list(initButtonList(app, main_layout)) {}

void CentralWidget::keyPressEvent(QKeyEvent* event) {
        // TODO Optimize this method, it contains a lot of unnecessary checks

        auto debugPrintKeybindings = [&]() {
                std::unordered_set<int> test_set;
                test_set.insert(keybindings.shutdown.cbegin(), keybindings.shutdown.cend());
                test_set.insert(keybindings.reboot.cbegin(), keybindings.reboot.cend());
                test_set.insert(keybindings.suspend.cbegin(), keybindings.suspend.cend());
                test_set.insert(keybindings.hibernate.cbegin(), keybindings.hibernate.cend());
                for (auto& item : test_set) qDebug() << "test set:" << item;
        };

        auto isPowerKey = [&](int key) -> bool {
                return (keybindings.shutdown.contains(key) || keybindings.reboot.contains(key)
                        || keybindings.suspend.contains(key)
                        || keybindings.hibernate.contains(key));
        };

        auto findButtonWithAction = [&](const QString&& action) -> PowerButton* {
                // TODO Too nested, clean up
                std::unordered_set<QString> acceptable_strings = {"PowerOff", "Reboot", "Suspend",
                                                                  "Hibernate"};

                if (!acceptable_strings.contains(action)) {
                        qFatal("Incorrect action passed! %s", action.toStdString().c_str());
                }

                if (!button_list)
                        qFatal("button_list is nullptr in %s, object destroyed prematurely!",
                               __func__);
                for (auto button : *button_list) {
                        //qDebug() << "Processing action" << button->getAction();
                        if (!button) qFatal("Some power buttons are null, terminating!");
                        if (button->getDBusAction() == action) {
                                qDebug() << "Button found in findButtonWithAction! Action is"
                                         << action << "and button's text() is" << button->text();
                                return button;
                        }
                }
                qFatal("No button associated with action %s, terminating!",
                       action.toStdString().c_str());
                return nullptr; // never meant to occur
        };
        auto initPowerButton = [&](Action& action) {
                // TODO Too nested, clean up
                if (action.key == Qt::Key_unknown) qFatal("Key missing in Action!");

                if (isPowerKey(action.key)) {
                        if (keybindings.shutdown.contains(action.key)) {
                                action.button = findButtonWithAction("PowerOff");
                        } else if (keybindings.reboot.contains(action.key)) {
                                action.button = findButtonWithAction("Reboot");
                        } else if (keybindings.suspend.contains(action.key)) {
                                action.button = findButtonWithAction("Suspend");
                        } else if (keybindings.hibernate.contains(action.key)) {
                                action.button = findButtonWithAction("Hibernate");
                        }

                        if (!action.button) qFatal("Button nullptr in initButton, terminating!");
                        else if (action.button)
                                qDebug() << "Button initialized correctly, text:"
                                         << action.button->text();
                } else {
                        qFatal() << "Passed button is not a power button, bad code!";
                }
        };

        qInfo() << "----------------------------------------";
        qInfo() << "INFO! keyPressEvent registered!";
        current_action.key = event->key();

        if (keybindings.quit.contains(current_action.key)) { // Quit key pressed
                if (last_action.button
                    && last_action.button->isFocused()) { // Last button focused, Quit key unselects it
                        last_action.button->setFocus(false);
                } else {
                        qDebug() << "Quit key pressed, quitting!";
                        QApplication::quit();
                }
        } else if ((current_action.key != last_action.key)
                   && isPowerKey(current_action.key)) { // new selection
                qDebug() << "current key and last key don't match";
                initPowerButton(current_action);
                // select current button
                qDebug() << current_action.button->text() << "selected!";
                current_action.button->setFocus(true);
                // unselect last button if valid, otherwise ignore because it doesn't exist anyway
                if (last_action.button) last_action.button->setFocus(false);
                else qDebug() << "last key is null!";
        } else if ((current_action.key == last_action.key)
                   && isPowerKey(current_action.key)) { // click recognized, both keys match
                // TODO Display errors returned by power action
                qDebug() << "current and previous keys match";
                initPowerButton(current_action);
                // animate click, proceed with power action
                qDebug() << current_action.button->text() << "clicked!";
                current_action.button->animateClick(); // includes emitting click
                current_action.button->setFocus(false);
                resetActions();
                return;
        }

        // Print current key combination
        qInfo() << "Current key combination:"
                << (last_action.key != Qt::Key_unknown ? QString::number(last_action.key) : "NULL")
                << QString::number(current_action.key);
        updateActions();
}

// TODO Consider QPointer
void CentralWidget::updateActions() {
        last_action.key       = current_action.key;
        last_action.button    = current_action.button;
        current_action.key    = Qt::Key_unknown;
        current_action.button = nullptr;
}

void CentralWidget::resetActions() {
        last_action.key       = Qt::Key_unknown;
        last_action.button    = nullptr;
        current_action.key    = Qt::Key_unknown;
        current_action.button = nullptr;
}
