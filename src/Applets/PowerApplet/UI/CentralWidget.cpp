/* Pretty Applets

   Copyright (C) 2025 Łukasz Wrodarczyk

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
#include "../../Config/Config.h"
#include "../../Config/Keys.h"

#include <QApplication>
#include <QDebug>

#include <cctype>
#include <vector>

using std::string;
using GlobalKeys = Keys::GlobalKeys;
using PowerKeys  = Keys::PowerAppletKeys;

static std::array<QIcon, 4> initButtonIcons() {
        Q_INIT_RESOURCE(Icons);
        std::array<QIcon, 4> button_icons{QIcon(":/Icons/Power/shutdown.svg"),
                                          QIcon(":/Icons/Power/reboot.svg"),
                                          QIcon(":/Icons/Power/suspend.svg"),
                                          QIcon(":/Icons/Power/hibernate.svg")};
        return button_icons;
}

static std::vector<PowerButton*> initButtonList(QBoxLayout* main_layout) {
        const auto& primary_buttons_data = Config::WindowLayoutProperties::getPrimaryPowerButtons();
        const auto  primary_buttons_icons = initButtonIcons();
        std::vector<PowerButton*> primary_buttons;
        transform(primary_buttons_data.begin(), primary_buttons_data.end(),
                  back_inserter(primary_buttons),
                  [&main_layout, &primary_buttons_icons,
                   &primary_buttons_data](const auto& data) -> PowerButton* {
                          qDebug() << "New data:" << data.identifier << ":" << data.text;

                          if (data.identifier.toLower() == "poweroff"
                              || data.identifier.toLower() == "shutdown") {
                                  QString method = "PowerOff";
                                  qDebug() << "Created PowerOff!";
                                  return new PowerButton(main_layout, primary_buttons_icons[0],
                                                         data.text, method);
                          } else if (data.identifier.toLower() == "reboot") {
                                  QString method = "Reboot";
                                  qDebug() << "Created Reboot!";
                                  return new PowerButton(main_layout, primary_buttons_icons[1],
                                                         data.text, method);
                          } else if (data.identifier.toLower() == "suspend") {
                                  QString method = "Suspend";
                                  qDebug() << "Created Suspend!";
                                  return new PowerButton(main_layout, primary_buttons_icons[2],
                                                         data.text, method);
                          } else if (data.identifier.toLower() == "hibernate") {
                                  QString method = "Hibernate";
                                  qDebug() << "Created Hibernate!";
                                  return new PowerButton(main_layout, primary_buttons_icons[3],
                                                         data.text, method);
                          } else {
                                  qCritical()
                                          << "Unknown button identifier detected!"
                                          << "Potential error on the line defining primary buttons!";
                          }
                  });

        if (!primary_buttons.empty()) {
                return primary_buttons;
        } else {
                qCritical() << "Empty button list!"
                            << "Check config for strings in 'primary_buttons'!"
                            << "Must not be empty!"; // TODO Defaults
        }
}

static bool isPowerKey(int key) {
        return (PowerKeys::getPrimaryButton1Keys().contains(key)
                || PowerKeys::getPrimaryButton2Keys().contains(key)
                || PowerKeys::getPrimaryButton3Keys().contains(key)
                || PowerKeys::getPrimaryButton4Keys().contains(key));
};

PowerButton* findButtonWithAction(std::vector<PowerButton*> buttons, const QString& action) {
        // TODO Too nested, clean up
        std::unordered_set<QString> acceptable_strings = {"PowerOff", "Reboot", "Suspend",
                                                          "Hibernate"};

        if (!acceptable_strings.contains(action)) {
                qFatal("Incorrect action passed! %s", action.toStdString().c_str());
        }

        for (auto button : buttons) {
                //qDebug() << "Processing action" << button->getAction();
                if (!button) { qFatal("Some power buttons are null, terminating!"); }
                if (button->getDBusAction() == action) {
                        qDebug() << "Button found in findButtonWithAction! Action is" << action
                                 << "and button's text() is" << button->text();
                        return button;
                }
        }

        qFatal("No button associated with action %s, terminating!", action.toStdString().c_str());
};

Action::Action(int key, PowerButton* button) : key(key), button(button) {}

void Action::reset() {
        key    = Qt::Key_unknown;
        button = nullptr;
}

void Action::updatePowerButton(std::vector<PowerButton*> buttons) {
        // TODO Too nested, clean up
        if (key == Qt::Key_unknown) { qFatal("Key missing in Action!"); }

        if (isPowerKey(key)) {
                if (PowerKeys::getPrimaryButton1Keys().contains(key)) {
                        button = findButtonWithAction(buttons, "PowerOff");
                } else if (PowerKeys::getPrimaryButton2Keys().contains(key)) {
                        button = findButtonWithAction(buttons, "Reboot");
                } else if (PowerKeys::getPrimaryButton3Keys().contains(key)) {
                        button = findButtonWithAction(buttons, "Suspend");
                } else if (PowerKeys::getPrimaryButton4Keys().contains(key)) {
                        button = findButtonWithAction(buttons, "Hibernate");
                }

                if (!button) {
                        qFatal("Button nullptr in initButton, terminating!");
                } else {
                        qDebug() << "Button initialized correctly, text:" << button->text();
                }
        } else {
                qFatal() << "Passed key is not a power key!";
        }
};

CentralWidget::CentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)), last_action(Qt::Key_unknown, nullptr),
        current_action(Qt::Key_unknown, nullptr), button_list(initButtonList(main_layout)) {
        if (!parent) {
                qFatal() << "Parent of CentralWidget is null in" << __func__
                         << "! Shutting down to avoid memory leaks...";
        }
}

void CentralWidget::keyPressEvent(QKeyEvent* event) {
        if (!event) {
                qWarning() << "Warning, received null keypress event in" << __func__;
                return;
        }
        qDebug() << "----------------------------------------";
        qDebug() << "keyPressEvent registered!";
        current_action.key = event->key();

        if (PowerKeys::getQuitKeys().contains(current_action.key)) { // Quit key pressed
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
                current_action.updatePowerButton(button_list);
                // select current button
                qDebug() << current_action.button->text() << "selected!";
                current_action.button->setFocus(true);
                // unselect last button if valid, otherwise ignore because it doesn't exist anyway
                if (last_action.button) {
                        last_action.button->setFocus(false);
                } else {
                        qDebug() << "last key is null!";
                }
        } else if ((current_action.key == last_action.key)
                   && isPowerKey(current_action.key)) { // click recognized, both keys match
                // TODO Display errors returned by power action
                qDebug() << "current and previous keys match";
                current_action.updatePowerButton(button_list);
                // animate click, proceed with power action
                qDebug() << current_action.button->text() << "clicked!";
                current_action.button->animateClick(); // includes emitting click
                current_action.button->setFocus(false);
                resetActions();
                return;
        }

        // Print current key combination
        qDebug() << "Current key combination:"
                 << (last_action.key != Qt::Key_unknown ? QString::number(last_action.key) : "NULL")
                 << QString::number(current_action.key);
        updateActions();
}

void CentralWidget::updateActions() {
        last_action    = std::move(current_action);
        current_action = Action(Qt::Key_unknown, nullptr);
}

void CentralWidget::resetActions() {
        last_action.reset();
        current_action.reset();
}
