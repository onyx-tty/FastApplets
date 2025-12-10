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

#include <QApplication>
#include <QDebug>

#include <unordered_map>
#include <vector>

using std::string, std::vector, std::array, std::unordered_map;
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

static bool isPowerKey(int& key) {
        for (const auto& button_keys : PowerKeys::getPrimaryButtonKeys()) {
                if (button_keys.contains(key)) { return true; }
        }

        return false;
};

KeyAction::KeyAction(int key, PowerButton* button) : key(key), button(button) {}

void KeyAction::reset() {
        key    = Qt::Key_unknown;
        button = nullptr;
}

void KeyAction::updatePowerButton(const vector<PowerButton*>& buttons) {
        const auto& button_properties = Config::WindowLayoutProperties::getPrimaryPowerButtons();

        if (button_properties.size() != buttons.size()) {
                qFatal("%s: button_properties (%s) larger than buttons (%s)!", __func__,
                       QString::number(button_properties.size()).toStdString().c_str(),
                       QString::number(buttons.size()).toStdString().c_str());
        }

        static const auto power_key_interpreter = [](int key) -> int {
                static const unordered_map<int, int> power_key_map{
                        {Qt::Key_1, 1},
                        {Qt::Key_2, 2},
                        {Qt::Key_3, 3},
                        {Qt::Key_4, 4}
                };

                return power_key_map.at(key);
        };

        for (size_t i = 0; i != buttons.size(); ++i) {
                if (power_key_interpreter(key) == button_properties[i].order) {
                        button = buttons[i];
                        qDebug() << "Button" << i << "updated to: " << button->getDBusAction();
                        return;
                }
        }

        qFatal("%s: button not found!", __func__);
}

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
        current_action.key = event->key();

        const auto current_action_updater = [this]() { current_action.updatePowerButton(button_list); };

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
                current_action_updater();
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
                current_action_updater();
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

void PowerCentralWidget::updateActions() {
        last_action    = std::move(current_action);
        current_action = KeyAction(Qt::Key_unknown, nullptr);
}

void PowerCentralWidget::resetActions() {
        last_action.reset();
        current_action.reset();
}
