// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionCentralWidget.h"
#include "Core/Config/ConfigFile/ActionApplet/ActionAppletConfig.h"
#include "Core/Config/KeysFile/ActionApplet/ActionAppletKeys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Widgets/ActionButton.h"

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

ActionButton* findActionButton(int key, std::vector<ActionButton*> buttons) {
        const auto iter = std::find_if(buttons.cbegin(), buttons.cend(),
                                       [key](const ActionButton* button) -> bool {
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
std::vector<ActionButton*> ActionCentralWidget::createButtons(const ActionAppletConfig& config,
                                                              const ActionAppletKeys&   keys,
                                                              const ActionAppletKeys& default_keys) {
        // TODO: Rename to action_button(s)_properties/data/keys
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

        std::vector<ActionButton*> primary_buttons = {};
        primary_buttons.reserve(primary_buttons_data.size());

        for (size_t i = 0; i != primary_buttons_data.size(); ++i) {
                QIcon       icon          = primary_buttons_data[i].icon;
                QString     text          = primary_buttons_data[i].text;
                QString     command       = primary_buttons_data[i].command;
                keybindings keys          = key_getter(i);
                auto*       action_button = new ActionButton{icon,
                                                             text,
                                                             keys,
                                                             command,
                                                             primary_button_properties,
                                                             this};

                layout()->addWidget(action_button);
                primary_buttons.push_back(action_button);
        }

        if (primary_buttons.empty()) { qFatal("No buttons found!"); }

        return primary_buttons;
}

// TODO: Accept buttons as param for dependency injection

ActionCentralWidget::ActionCentralWidget(const ActionAppletConfig& config,
                                         const ActionAppletKeys&   keys,
                                         const ActionAppletKeys& default_keys, QWidget* parent) :
        QWidget(parent), quit_keys(keys.getQuit()) {
        setLayout(new QHBoxLayout(this));
        buttons = createButtons(config, keys, default_keys);
}

const std::vector<ActionButton*>& ActionCentralWidget::getButtons() const {
        return buttons;
}

void ActionCentralWidget::keyPressEvent(QKeyEvent* event) {
        int key = event->key();

        // Quit pressed
        if (isQuitKey(key, quit_keys)) {
                // Unselect if a button is focused
                if (auto* focused = qobject_cast<ActionButton*>(QApplication::focusWidget())) {
                        focused->clearFocus();
                        this->setFocus();
                } else { // Quit if not
                        QApplication::quit();
                }
        } else if (auto* action_button = findActionButton(key, buttons)) { // ActionButton pressed
                // Click if already focused
                if (action_button->hasFocus()) {
                        action_button->animateClick();
                        action_button->clearFocus();
                        this->setFocus();
                } else { // Re-focus if not
                        if (auto* focused = qobject_cast<ActionButton*>(
                                    QApplication::focusWidget())) {
                                focused->clearFocus();
                        }
                        action_button->setFocus(Qt::FocusReason::MouseFocusReason);
                }
        }
}

void ActionCentralWidget::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);

        if (auto* focused = qobject_cast<ActionButton*>(QApplication::focusWidget())) {
                focused->clearFocus();
        }
        this->setFocus();
}
