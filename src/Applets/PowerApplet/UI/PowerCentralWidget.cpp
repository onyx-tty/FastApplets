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
#include "UI/Enums/ButtonIDs.h"

#include <qnamespace.h>
#include <qobject.h>
#include <unordered_map>
#include <QApplication>

using button_bindings     = std::unordered_map<const PowerButton*, const keybindings*>;
using keybinding_bindings = std::unordered_map<const keybindings*, PowerButton*>;

static std::array<QIcon, 4> createButtonIcons() {
        Q_INIT_RESOURCE(Icons);
        std::array<QIcon, 4> button_icons{QIcon(":/Icons/Power/shutdown.svg"),
                                          QIcon(":/Icons/Power/reboot.svg"),
                                          QIcon(":/Icons/Power/suspend.svg"),
                                          QIcon(":/Icons/Power/hibernate.svg")};
        return button_icons;
}

static bool isPowerKey(int key) {
        for (const auto& button_keys : Keys::getKeys().getPowerAppletKeys().getPrimaryButtonKeys()) {
                if (button_keys.contains(key)) { return true; }
        }

        return false;
}

static bool isQuitKey(int key) {
        const auto& quit_keys = Keys::getKeys().getPowerAppletKeys().getQuitKeys();

        return quit_keys.contains(key);
}

const keybindings& PowerCentralWidget::getKeysFromPowerButton(const PowerButton* power_button) {
        static button_bindings map       = {};
        bool                   processed = false;

        if (!processed) {
                const auto& keys = Keys::getKeys().getPowerAppletKeys().getPrimaryButtonKeys();

                // Cache the full keys map
                for (size_t i = 0; i < button_list.size() && i < keys.size(); ++i) {
                        map[button_list[i]] = &keys[i];
                }

                processed = true;
        }

        return *(map.at(power_button));
}

PowerButton* PowerCentralWidget::getPowerButtonFromKeys(const keybindings& keys) {
        static keybinding_bindings map       = {};
        bool                       processed = false;

        // TODO Simply reuse the map in getKeysFromPowerButton
        if (!processed) {
                const auto& keys = Keys::getKeys().getPowerAppletKeys().getPrimaryButtonKeys();

                // Cache the full PowerButton map
                for (size_t i = 0; i < button_list.size() && i < keys.size(); ++i) {
                        map[&keys[i]] = button_list[i];
                }

                processed = true;
        }

        // TODO Needs a safety check for missing keys - improved isPowerKey
        return map.at(&keys);
}

PowerButton* PowerCentralWidget::getPowerButtonFromKey(int key) {
        static std::unordered_map<int, PowerButton*> map       = {};
        bool                                         processed = false;

        const auto& keys_arr = Keys::getKeys().getPowerAppletKeys().getPrimaryButtonKeys();
        if (!processed) {
                // Cache the full keys map
                for (const auto& keys : keys_arr) { map[key] = getPowerButtonFromKeys(keys); }

                processed = true;
        }

        for (const auto& keys : keys_arr) {
                if (keys.contains(key)) { return getPowerButtonFromKeys(keys); }
        }

        return nullptr;
}

PowerButton* PowerCentralWidget::getPowerButtonFromPowerButtonID(power_button_id button) {
        for (auto* power_button : button_list) {
                if (power_button->getIdentifier() == button) { return power_button; }
        }

        // TODO In CppUtils, qt::qstring_utils::toQString should be able to interpret enums
        QFATAL("Received unexpected power_button_id %i!", static_cast<int>(button));
}

const keybindings& PowerCentralWidget::getKeysFromPowerButtonID(power_button_id button) {
        const auto* power_button = getPowerButtonFromPowerButtonID(button);
        return getKeysFromPowerButton(power_button);
}

power_button_id PowerCentralWidget::getPowerButtonIDFromKeys(const keybindings& keys) {
        const auto* power_button = getPowerButtonFromKeys(keys);

        return power_button->getIdentifier();
}

power_button_id PowerCentralWidget::getPowerButtonIDFromKey(int key) {
        const auto* power_button = getPowerButtonFromKey(key);

        return power_button->getIdentifier();
}

// TODO Split and simplify this
std::vector<PowerButton*> PowerCentralWidget::createButtonList(QBoxLayout* main_layout) {
        const auto& primary_buttons_data =
                Config::getConfig().getWindowLayoutProperties().getPrimaryPowerButtons();
        const auto                primary_buttons_icons = createButtonIcons();
        std::vector<PowerButton*> primary_buttons;
        for (const auto& primary_data : primary_buttons_data) {
                QDEBUG() << "New primary button data:" << primary_data.text;

                if (primary_data.identifier == power_button_id::shutdown) {
                        QString method = "PowerOff";
                        QDEBUG() << "Created PowerOff button!";
                        primary_buttons.push_back(new PowerButton(main_layout,
                                                                  primary_data.identifier,
                                                                  primary_buttons_icons[0],
                                                                  primary_data.text, method));
                } else if (primary_data.identifier == power_button_id::reboot) {
                        QString method = "Reboot";
                        QDEBUG() << "Created Reboot button!";
                        primary_buttons.push_back(new PowerButton(main_layout,
                                                                  primary_data.identifier,
                                                                  primary_buttons_icons[1],
                                                                  primary_data.text, method));
                } else if (primary_data.identifier == power_button_id::suspend) {
                        QString method = "Suspend";
                        QDEBUG() << "Created Suspend button!";
                        primary_buttons.push_back(new PowerButton(main_layout,
                                                                  primary_data.identifier,
                                                                  primary_buttons_icons[2],
                                                                  primary_data.text, method));
                } else if (primary_data.identifier == power_button_id::hibernate) {
                        QString method = "Hibernate";
                        QDEBUG() << "Created Hibernate button!";
                        primary_buttons.push_back(new PowerButton(main_layout,
                                                                  primary_data.identifier,
                                                                  primary_buttons_icons[3],
                                                                  primary_data.text, method));
                } else {
                        QCRITICAL() << "Unknown button identifier detected!";
                }
        };

        if (!primary_buttons.empty()) {
                return primary_buttons;
        } else {
                // TODO Use defaults instead
                QFATAL("No buttons found in button_list!");
        }
}

PowerCentralWidget::PowerCentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)),
        button_list(createButtonList(main_layout)) {
        if (!parent) {
                QFATAL("Parent of PowerCentralWidget is null! Shutting down to avoid memory leaks...");
        }
}

const QBoxLayout* PowerCentralWidget::getMainLayout() const {
        return main_layout;
}

const std::vector<PowerButton*>& PowerCentralWidget::getButtonList() const {
        return button_list;
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        if (!event) {
                QCRITICAL() << "Received nullptr event!";
                return;
        }

        PowerButton* current = getPowerButtonFromKey(event->key());

        // Quit pressed
        if (isQuitKey(event->key())) {
                // unselect if a button is focused
                if (auto* focused = qobject_cast<PowerButton*>(QApplication::focusWidget())) {
                        focused->clearFocus();
                        this->setFocus();
                } else { // quit if not
                        QApplication::quit();
                }
        } else if (current) { // PowerButton pressed
                if (current->hasFocus()) {
                        current->animateClick();
                        current->clearFocus();
                        this->setFocus();
                } else {
                        if (auto* focused = qobject_cast<PowerButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                        }
                        current->setFocus(Qt::FocusReason::MouseFocusReason);
                }
        } else {
                QDEBUG() << "Unrecognized key pressed!";
        }
}
