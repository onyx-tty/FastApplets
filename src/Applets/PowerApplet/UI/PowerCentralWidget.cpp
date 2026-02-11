/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

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
#include "Core/Log.h"
#include "UI/Input/KeyAction.h"

#include <QApplication>

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

// TODO Split and simplify this
static vector<PowerButton*> initButtonList(QBoxLayout* main_layout) {
        const auto& primary_buttons_data = Config::WindowLayoutProperties::getPrimaryPowerButtons();
        const auto  primary_buttons_icons = initButtonIcons();
        vector<PowerButton*> primary_buttons;
        for_each(primary_buttons_data.begin(), primary_buttons_data.end(),
                 [&primary_buttons, &main_layout, &primary_buttons_icons,
                  &primary_buttons_data](const auto& data) {
                         QDEBUG() << "New data:" << data.identifier << ":" << data.text;

                         if (data.identifier.toLower() == "poweroff"
                             || data.identifier.toLower() == "shutdown") {
                                 QString method = "PowerOff";
                                 QDEBUG() << "Created PowerOff!";
                                 primary_buttons.push_back(new PowerButton(main_layout,
                                                                           primary_buttons_icons[0],
                                                                           data.text, method));
                         } else if (data.identifier.toLower() == "reboot") {
                                 QString method = "Reboot";
                                 QDEBUG() << "Created Reboot!";
                                 primary_buttons.push_back(new PowerButton(main_layout,
                                                                           primary_buttons_icons[1],
                                                                           data.text, method));
                         } else if (data.identifier.toLower() == "suspend") {
                                 QString method = "Suspend";
                                 QDEBUG() << "Created Suspend!";
                                 primary_buttons.push_back(new PowerButton(main_layout,
                                                                           primary_buttons_icons[2],
                                                                           data.text, method));
                         } else if (data.identifier.toLower() == "hibernate") {
                                 QString method = "Hibernate";
                                 QDEBUG() << "Created Hibernate!";
                                 primary_buttons.push_back(new PowerButton(main_layout,
                                                                           primary_buttons_icons[3],
                                                                           data.text, method));
                         } else {
                                 QCRITICAL()
                                         << "Unknown button identifier detected!"
                                         << "Potential error on the line defining primary buttons!";
                         }
                 });

        if (!primary_buttons.empty()) {
                return primary_buttons;
        } else {
                // TODO Use defaults instead
                QFATAL("Empty button list! Check config for strings in 'primary_buttons'! Must not be empty!");
        }
}

static bool isPowerKey(int key) {
        for (const auto& button_keys : PowerKeys::getPrimaryButtonKeys()) {
                if (button_keys.contains(key)) { return true; }
        }

        return false;
};

PowerCentralWidget::PowerCentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)),
        button_list(initButtonList(main_layout)) {
        if (!parent) {
                QFATAL("Parent of PowerCentralWidget is null! Shutting down to avoid memory leaks...");
        }
}

namespace {
enum key_sequence_index { previous = 0, current = 1 };
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        if (!event) {
                QWARNING() << "Warning, received null keypress event";
                return;
        }

        QDEBUG() << "----------------------------------------";
        QDEBUG() << "keyPressEvent registered!";

        // TODO Refactor to universally call updateActions without out the out-of-range error requiring updateActionsUnsafe
        // New key press event, push the previous one to the back
        //updateActions(event->key());
        // workaround:
        if (isPowerKey(event->key())) {
                updateActions(event->key());
        } else {
                updateActionsUnsafe(event->key(), nullptr);
        }

        // if quit pressed
        if (PowerKeys::getQuitKeys().contains(key_action_sequence[current].getKey())) {
                // button focus active, quit key = unselect
                if (key_action_sequence[previous].getButton()
                    && key_action_sequence[previous].getButton()->isFocused()) {
                        key_action_sequence[previous].debugGetButtonNonConst()->setFocus(false);
                        // no selection active, quit key = terminate application
                } else {
                        QDEBUG() << "Quit key pressed, quitting!";
                        QApplication::quit();
                }
                // mismatched sequence, remove focus, select another button if key matches
        } else if ((key_action_sequence[current].getKey() != key_action_sequence[previous].getKey())
                   && isPowerKey(key_action_sequence[current].getKey())) { // new selection
                QDEBUG() << "current key and last key don't match";
                // select current button
                QDEBUG() << key_action_sequence[current].getButton()->text() << "selected!";
                key_action_sequence[current].debugGetButtonNonConst()->setFocus(true);
                // unselect last button if valid, otherwise ignore because it doesn't exist anyway
                if (key_action_sequence[previous].getButton()) {
                        key_action_sequence[previous].debugGetButtonNonConst()->setFocus(false);
                } else {
                        QDEBUG() << "last key is null!";
                }
                // recurring sequence, activate button
        } else if ((key_action_sequence[current].getKey() == key_action_sequence[previous].getKey())
                   && isPowerKey(key_action_sequence[current]
                                         .getKey())) { // click recognized, both keys match
                // TODO Display errors returned by power action
                QDEBUG() << "current and previous keys match";
                // animate click, proceed with power action
                QDEBUG() << key_action_sequence[current].getButton()->text() << "clicked!";
                key_action_sequence[current]
                        .debugGetButtonNonConst()
                        ->animateClick(); // includes emitting click
                key_action_sequence[current].debugGetButtonNonConst()->setFocus(false);
                resetActions();
                return;
        }

        // Print current key combination
        QDEBUG() << "Current key combination:"
                 << (key_action_sequence[previous].getKey() != Qt::Key_unknown
                             ? QString::number(key_action_sequence[previous].getKey())
                             : "NULL")
                 << QString::number(key_action_sequence[current].getKey());
}

void PowerCentralWidget::updateActions(int new_key) {
        key_action_sequence[previous] = std::move(key_action_sequence[current]);
        key_action_sequence[current]  = KeyAction(button_list, new_key);
}

void PowerCentralWidget::updateActionsUnsafe(int key, PowerButton* button) {
        key_action_sequence[previous] = std::move(key_action_sequence[current]);
        key_action_sequence[current]  = KeyAction(key, button);
}

void PowerCentralWidget::resetActions() {
        key_action_sequence[previous].reset();
        key_action_sequence[current].reset();
}
