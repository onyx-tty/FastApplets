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
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Log/Log.h"
#include "UI/Enums/ButtonIDs.h"
#include "Widgets/PowerButton.h"

#include <array>
#include <cstddef>
#include <qnamespace.h>
#include <qobject.h>
#include <qtresource.h>
#include <unordered_map>
#include <vector>
#include <QApplication>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

// TODO Store a {PowerButton*, keybindings*} map in a more visible place
using button_bindings     = std::unordered_map<const PowerButton*, const keybindings*>;
using keybinding_bindings = std::unordered_map<const keybindings*, PowerButton*>;

// TODO Avoid creating icons for uninitialized buttons
static std::array<QIcon, 4> createButtonIcons() {
        Q_INIT_RESOURCE(Icons);
        std::array<QIcon, 4> button_icons{QIcon(":/Icons/Power/shutdown.svg"),
                                          QIcon(":/Icons/Power/reboot.svg"),
                                          QIcon(":/Icons/Power/suspend.svg"),
                                          QIcon(":/Icons/Power/hibernate.svg")};
        return button_icons;
}

// TODO Extract
static bool isPowerKey(int key) {
        for (const auto& button_keys : PowerAppletKeys::get().getPrimaryButtonKeys()) {
                if (button_keys.contains(key)) { return true; }
        }

        return false;
}

static bool isQuitKey(int key) {
        const auto& quit_keys = PowerAppletKeys::get().getQuitKeys();

        return quit_keys.contains(key);
}

const keybindings& PowerCentralWidget::getKeysFromPowerButton(const PowerButton* power_button) {
        static button_bindings map       = {};
        static bool            processed = false;

        if (!processed) {
                const auto& keys = PowerAppletKeys::get().getPrimaryButtonKeys();

                // Cache the full keys map
                for (size_t i = 0; i < buttons.size() && i < keys.size(); ++i) {
                        map[buttons[i]] = &keys[i];
                }

                processed = true;
        }

        return *(map.at(power_button));
}

PowerButton* PowerCentralWidget::getPowerButtonFromKeys(const keybindings& keys) {
        static keybinding_bindings map       = {};
        static bool                processed = false;

        // TODO Simply reuse the map in getKeysFromPowerButton
        if (!processed) {
                const auto& keys = PowerAppletKeys::get().getPrimaryButtonKeys();

                // Cache the full PowerButton map
                for (size_t i = 0; i < buttons.size() && i < keys.size(); ++i) {
                        map[&keys[i]] = buttons[i];
                }

                processed = true;
        }

        // TODO Needs a safety check for missing keys - improved isPowerKey
        return map.at(&keys);
}

PowerButton* PowerCentralWidget::getPowerButtonFromKey(int key) {
        static std::unordered_map<int, PowerButton*> map       = {};
        static bool                                  processed = false;

        const auto& keys_arr = PowerAppletKeys::get().getPrimaryButtonKeys();
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
        for (auto* power_button : buttons) {
                if (power_button->getID() == button) { return power_button; }
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

        return power_button->getID();
}

power_button_id PowerCentralWidget::getPowerButtonIDFromKey(int key) {
        const auto* power_button = getPowerButtonFromKey(key);

        return power_button->getID();
}

QString getDBusMethodFromPowerButtonID(power_button_id id) {
        const std::unordered_map<power_button_id, QString> map =
                {{power_button_id::shutdown, "PowerOff"},
                 {power_button_id::reboot, "Reboot"},
                 {power_button_id::suspend, "Suspend"},
                 {power_button_id::hibernate, "Hibernate"}};

        if (!map.contains(id)) {
                QFATAL("Received invalid power_button_id %i!", static_cast<int>(id));
        }

        return map.at(id);
}

std::vector<PowerButton*> PowerCentralWidget::createButtons(QBoxLayout* main_layout) {
        const auto& primary_buttons_data =
                PowerAppletConfig::get().getLayoutProperties().getPowerButtons();
        const auto primary_buttons_icons = createButtonIcons();

        if (primary_buttons_icons.size() != primary_buttons_data.size()
            && primary_buttons_icons.size() != 4) {
                QFATAL("primary_buttons_icons mismatched, 4 icons expected!");
        }

        std::vector<PowerButton*> primary_buttons;
        primary_buttons.reserve(primary_buttons_data.size());

        for (size_t i = 0; i != primary_buttons_data.size(); ++i) {
                QDEBUG() << "Created" << primary_buttons_data[i].label << "!";
                QString method = getDBusMethodFromPowerButtonID(primary_buttons_data[i].id);
                primary_buttons.push_back(new PowerButton(main_layout, primary_buttons_data[i].id,
                                                          primary_buttons_icons[i],
                                                          primary_buttons_data[i].label, method,
                                                          primary_buttons_data[i].command));
        }

        if (primary_buttons.empty()) { QFATAL("No buttons found!"); }

        return primary_buttons;
}

PowerCentralWidget::PowerCentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)), buttons(createButtons(main_layout)) {
        if (!parent) {
                QFATAL("Parent of PowerCentralWidget is null! Shutting down to avoid memory leaks...");
        }
}

const QBoxLayout* PowerCentralWidget::getMainLayout() const {
        return main_layout;
}

const std::vector<PowerButton*>& PowerCentralWidget::getButtons() const {
        return buttons;
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        PowerButton* current = getPowerButtonFromKey(event->key());

        // Quit pressed
        if (isQuitKey(event->key())) {
                // Unselect if a button is focused
                if (auto* focused = qobject_cast<PowerButton*>(QApplication::focusWidget())) {
                        focused->clearFocus();
                        this->setFocus();
                } else { // Quit if not
                        QApplication::quit();
                }
        } else if (current) { // PowerButton pressed
                // Click if already focused
                if (current->hasFocus()) {
                        current->animateClick();
                        current->clearFocus();
                        this->setFocus();
                } else { // Re-focus if not
                        if (auto* focused = qobject_cast<PowerButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                        }
                        current->setFocus(Qt::FocusReason::MouseFocusReason);
                }
        }
}
