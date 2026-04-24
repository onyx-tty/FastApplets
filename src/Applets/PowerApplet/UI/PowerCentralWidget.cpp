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
#include <unordered_map>
#include <vector>
#include <QApplication>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QObject>
#include <Qt>

// TODO Store a {PowerButton*, keybindings*} map in a more visible place
using button_bindings     = std::unordered_map<const PowerButton*, const keybindings*>;
using keybinding_bindings = std::unordered_map<const keybindings*, PowerButton*>;

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

PowerButtonRecords PowerCentralWidget::createButtons() {
        const auto& primary_buttons_data =
                PowerAppletConfig::get().getLayoutProperties().getPowerButtons();
        const std::vector<keybindings>  primary_button_keys = PowerAppletKeys::get()
                                                                      .getPrimaryButtonKeys();
        const std::vector<keybindings>& default_primary_button_keys =
                PowerAppletKeys::getDefault().getPrimaryButtonKeys();

        // TODO If applied key is already used elsewhere, there will be confusion
        //      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
        //      primary button 4 has missing keybinding, both buttons will be assigned to Qt_Key4.
        //      There should be a validation system in place for all keybindings, for example a
        //      set with all keys which have already been exhausted.
        const auto key_getter = [&primary_buttons_data, &primary_button_keys,
                                 &default_primary_button_keys](size_t i) -> keybindings {
                if (i < primary_button_keys.size()) { return primary_button_keys[i]; }
                if (i < default_primary_button_keys.size()) {
                        QWARNING()
                                << QString("Key for button %1 not found, applying default Qt_Key%1!")
                                           .arg(i + 1);
                        return default_primary_button_keys[i];
                }

                QCRITICAL() << "Number of buttons exceeds size of default keys! Buttons found:"
                            << primary_buttons_data.size();
                return keybindings{Qt::Key_unknown};
        };

        PowerButtonRecords primary_buttons{};
        primary_buttons.reserve(primary_buttons_data.size());

        for (size_t i = 0; i != primary_buttons_data.size(); ++i) {
                power_button_id id          = primary_buttons_data[i].id;
                QIcon           icon        = primary_buttons_data[i].icon;
                QString         text        = primary_buttons_data[i].text;
                QString         dbus_method = primary_buttons_data[i].dbus_method;
                ShellCommand    command     = primary_buttons_data[i].command;
                auto* power_button          = new PowerButton(id, icon, text, dbus_method, command);
                const keybindings& keys     = key_getter(i);

                main_layout->addWidget(power_button);
                primary_buttons.push_back({id, power_button, keys});
        }

        if (primary_buttons.empty()) { QFATAL("No buttons found!"); }

        return primary_buttons;
}

PowerCentralWidget::PowerCentralWidget() :
        main_layout(new QHBoxLayout(this)), buttons(createButtons()) {}

const QBoxLayout* PowerCentralWidget::getMainLayout() const {
        return main_layout;
}

const PowerButtonRecords& PowerCentralWidget::getButtons() const {
        return buttons;
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        int        current_key = event->key();
        const auto found_key = std::find_if(buttons.cbegin(), buttons.cend(),
                                            [current_key](const PowerButtonRecord& record) -> bool {
                                                    return record.keys.contains(current_key);
                                            });
        PowerButton* current = found_key != buttons.cend() ? found_key->widget : nullptr;

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
