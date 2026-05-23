// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerCentralWidget.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/Types/ButtonType.h"
#include "Widgets/PowerButton.h"

#include <algorithm>
#include <cstddef>
#include <vector>
#include <QApplication>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QObject>
#include <QShowEvent>
#include <QWidget>
#include <Qt>

namespace {

bool isPowerKey(int key) {
        for (const auto& button_keys : PowerAppletKeys::get().getPrimaryButton()) {
                if (button_keys.contains(key)) { return true; }
        }

        return false;
}

bool isQuitKey(int key) {
        const auto& quit_keys = PowerAppletKeys::get().getQuit();

        return quit_keys.contains(key);
}

} // namespace

std::vector<PowerButton*> PowerCentralWidget::createButtons() {
        const auto& primary_buttons_data =
                PowerAppletConfig::get().getLayoutProperties().getPowerButtons();
        const std::vector<keybindings>  primary_button_keys         = PowerAppletKeys::get()
                                                                              .getPrimaryButton();
        const std::vector<keybindings>& default_primary_button_keys = PowerAppletKeys::getDefault()
                                                                              .getPrimaryButton();

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

        std::vector<PowerButton*> primary_buttons{};
        primary_buttons.reserve(primary_buttons_data.size());

        for (size_t i = 0; i != primary_buttons_data.size(); ++i) {
                power_button_type type         = primary_buttons_data[i].type;
                QIcon             icon         = primary_buttons_data[i].icon;
                QString           text         = primary_buttons_data[i].text;
                QString           command      = primary_buttons_data[i].command;
                keybindings       keys         = key_getter(i);
                auto*             power_button = new PowerButton{type, icon, text, keys, command};

                layout()->addWidget(power_button);
                primary_buttons.push_back(power_button);
        }

        if (primary_buttons.empty()) { QFATAL("No buttons found!"); }

        return primary_buttons;
}

PowerCentralWidget::PowerCentralWidget() {
        setLayout(new QHBoxLayout(this));
        buttons = createButtons();
}

const std::vector<PowerButton*>& PowerCentralWidget::getButtons() const {
        return buttons;
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        // TODO Improve messy finding logic and adaptation from iter to ptr
        int          key          = event->key();
        const auto   found_button = std::find_if(buttons.cbegin(), buttons.cend(),
                                                 [key](const PowerButton* button) -> bool {
                                                       if (!button) { return false; }
                                                       return button->getKeys().contains(key);
                                                 });
        PowerButton* power_button = found_button != buttons.cend() ? *found_button : nullptr;

        // Quit pressed
        if (isQuitKey(key)) {
                // Unselect if a button is focused
                if (auto* focused = qobject_cast<PowerButton*>(QApplication::focusWidget())) {
                        focused->clearFocus();
                        this->setFocus();
                } else { // Quit if not
                        QApplication::quit();
                }
        } else if (power_button) { // PowerButton pressed
                // Click if already focused
                if (power_button->hasFocus()) {
                        power_button->animateClick();
                        power_button->clearFocus();
                        this->setFocus();
                } else { // Re-focus if not
                        if (auto* focused = qobject_cast<PowerButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                        }
                        power_button->setFocus(Qt::FocusReason::MouseFocusReason);
                }
        }
}

void PowerCentralWidget::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);

        if (auto* focused = qobject_cast<PowerButton*>(QApplication::focusWidget())) {
                focused->clearFocus();
        }
        this->setFocus();
}
