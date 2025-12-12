/* FastApplets

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

#include "PowerCentralWidget.h"
#include "Config/Config.h"
#include "Config/Keys.h"
#include "UI/Input/KeyAction.h"

#include <QApplication>
#include <QDebug>

#include <vector>

using std::string, std::vector, std::array;
using GlobalKeys = Keys::GlobalKeys;
using PowerKeys  = Keys::PowerAppletKeys;

static array<QIcon, 4> initButtonIcons() {
        Q_INIT_RESOURCE(Icons);
        array<QIcon, 4> button_icons{QIcon(":/Icons/Power/shutdown.svg"),
                                     QIcon(":/Icons/Power/reboot.svg"),
                                     QIcon(":/Icons/Power/suspend.svg"),
                                     QIcon(":/Icons/Power/hibernate.svg")};
        return button_icons;
}

static vector<PowerButton*> initButtonList(QBoxLayout* main_layout) {
        const auto& primary_buttons_data = Config::WindowLayoutProperties::getPrimaryPowerButtons();
        const auto  primary_buttons_icons = initButtonIcons();
        vector<PowerButton*> primary_buttons;
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
        for (const auto& button_keys : PowerKeys::getPrimaryButtonKeys()) {
                if (button_keys.contains(key)) { return true; }
        }

        return false;
};

PowerCentralWidget::PowerCentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)), last_action(Qt::Key_unknown, nullptr),
        current_action(Qt::Key_unknown, nullptr), button_list(initButtonList(main_layout)) {
        if (!parent) {
                qFatal() << "Parent of PowerCentralWidget is null in" << __func__
                         << "! Shutting down to avoid memory leaks...";
        }
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        if (!event) {
                qWarning() << "Warning, received null keypress event in" << __func__;
                return;
        }

        qDebug() << "----------------------------------------";
        qDebug() << "keyPressEvent registered!";
        current_action = KeyAction(event->key(), nullptr);

        const auto current_action_updater = [this, &event]() {
                current_action.updatePowerButton(event->key(), button_list);
        };

        if (PowerKeys::getQuitKeys().contains(current_action.getKey())) { // Quit key pressed
                if (last_action.getButton()
                    && last_action.getButton()->isFocused()) { // Last button focused, Quit key unselects it
                        last_action.debugGetButtonNonConst()->setFocus(false);
                } else {
                        qDebug() << "Quit key pressed, quitting!";
                        QApplication::quit();
                }
        } else if ((current_action.getKey() != last_action.getKey())
                   && isPowerKey(current_action.getKey())) { // new selection
                qDebug() << "current key and last key don't match";
                current_action_updater();
                // select current button
                qDebug() << current_action.getButton()->text() << "selected!";
                current_action.debugGetButtonNonConst()->setFocus(true);
                // unselect last button if valid, otherwise ignore because it doesn't exist anyway
                if (last_action.getButton()) {
                        last_action.debugGetButtonNonConst()->setFocus(false);
                } else {
                        qDebug() << "last key is null!";
                }
        } else if ((current_action.getKey() == last_action.getKey())
                   && isPowerKey(current_action.getKey())) { // click recognized, both keys match
                // TODO Display errors returned by power action
                qDebug() << "current and previous keys match";
                current_action_updater();
                // animate click, proceed with power action
                qDebug() << current_action.getButton()->text() << "clicked!";
                current_action.debugGetButtonNonConst()->animateClick(); // includes emitting click
                current_action.debugGetButtonNonConst()->setFocus(false);
                resetActions();
                return;
        }

        // Print current key combination
        qDebug() << "Current key combination:"
                 << (last_action.getKey() != Qt::Key_unknown ? QString::number(last_action.getKey()) : "NULL")
                 << QString::number(current_action.getKey());
        updateActions();
}

void PowerCentralWidget::updateActions() {
        last_action    = std::move(current_action);
        current_action = KeyAction(Qt::Key_unknown, nullptr);
}

void PowerCentralWidget::resetActions() {
        last_action.reset();
        current_action.reset();
}
