// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerCentralWidget.h"
#include "Core/Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Core/Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Widgets/PowerButton.h"

#include <algorithm>
#include <cstddef>
#include <vector>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QObject>
#include <QShowEvent>
#include <QWidget>
#include <Qt>
#include <QtGlobal>

namespace {

PowerButton* findPowerButton(int key, std::vector<PowerButton*> buttons) {
        const auto iter = std::find_if(buttons.cbegin(), buttons.cend(),
                                       [key](const PowerButton* button) -> bool {
                                               if (!button) { return false; }
                                               return button->getKeys().contains(key);
                                       });

        return iter != buttons.cend() ? *iter : nullptr;
}

bool isQuitKey(int key, const keybindings& quit_keys) {
        return quit_keys.contains(key);
}

} // namespace

// TODO: Accept button as param for dependency injection
std::vector<PowerButton*> PowerCentralWidget::createButtons(const PowerAppletConfig& config,
                                                            const PowerAppletKeys&   keys,
                                                            const PowerAppletKeys&   default_keys) {
        // TODO: Rename to power_button(s)_properties/data/keys
        const auto& primary_button_properties = config.getPrimaryButtonProperties();
        const auto& primary_buttons_data      = config.getLayoutProperties().getPrimaryButtons();
        const std::vector<keybindings>& primary_button_keys = keys.getPrimaryButton();
        const std::vector<keybindings>& default_primary_button_keys = default_keys.getPrimaryButton();

        // TODO If applied key is already used elsewhere, there will be confusion
        //      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
        //      primary button 4 has missing keybinding, both buttons will be assigned to Qt_Key4.
        //      There should be a validation system in place for all keybindings, for example a
        //      set with all keys which have already been exhausted.
        const auto key_getter = [&primary_buttons_data, &primary_button_keys,
                                 &default_primary_button_keys](size_t i) -> keybindings {
                if (i < primary_button_keys.size()) { return primary_button_keys[i]; }
                if (i < default_primary_button_keys.size()) {
                        qWarning()
                                << QString("Key for button %1 not found, applying default Qt_Key%1!")
                                           .arg(i + 1);
                        return default_primary_button_keys[i];
                }

                qCritical() << "Number of buttons exceeds size of default keys! Buttons found:"
                            << primary_buttons_data.size();
                return keybindings{Qt::Key_unknown};
        };

        std::vector<PowerButton*> primary_buttons = {};
        primary_buttons.reserve(primary_buttons_data.size());

        for (size_t i = 0; i != primary_buttons_data.size(); ++i) {
                power_button_type type    = primary_buttons_data[i].type;
                QIcon             icon    = primary_buttons_data[i].icon;
                QString           text    = primary_buttons_data[i].text;
                QString           command = primary_buttons_data[i].command;
                keybindings       keys    = key_getter(i);
                auto* power_button = new PowerButton{type, icon,    text,
                                                     keys, command, primary_button_properties,
                                                     this};

                layout()->addWidget(power_button);
                primary_buttons.push_back(power_button);
        }

        if (primary_buttons.empty()) { qFatal("No buttons found!"); }

        return primary_buttons;
}

// TODO: Accept buttons as param for dependency injection

PowerCentralWidget::PowerCentralWidget(const PowerAppletConfig& config, const PowerAppletKeys& keys,
                                       const PowerAppletKeys& default_keys, QWidget* parent) :
        QWidget(parent), quit_keys(keys.getQuit()),
        double_key_press(config.getPrimaryButtonProperties().getDoubleKeyPress()) {
        setLayout(new QHBoxLayout(this));
        buttons = createButtons(config, keys, default_keys);
}

const std::vector<PowerButton*>& PowerCentralWidget::getButtons() const {
        return buttons;
}

void PowerCentralWidget::keyPressEvent(QKeyEvent* event) {
        int key = event->key();

        if (double_key_press) {
                // Quit pressed
                if (isQuitKey(key, quit_keys)) {
                        // Unselect if a button is focused
                        if (auto* focused = qobject_cast<PowerButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                                this->setFocus();
                        } else { // Quit if not
                                QApplication::quit();
                        }
                } else if (auto* power_button = findPowerButton(key,
                                                                buttons)) { // PowerButton pressed
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
        } else {
                if (isQuitKey(key, quit_keys)) {
                        QApplication::quit();
                } else if (auto* power_button = findPowerButton(key, buttons)) {
                        power_button->animateClick();
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
