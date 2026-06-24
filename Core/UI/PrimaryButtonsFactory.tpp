// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "PrimaryButtonsFactory.h"

#include <cstddef>
#include <vector>
#include <QDebug>
#include <QIcon>
#include <QString>
#include <Qt>
#include <QtGlobal>

class PrimaryButton;

template<applet::type TApplet>
std::vector<PrimaryButton*> PrimaryButtonsFactory<TApplet>::create(const TConfig& config,
                                                                   const TKeys&   keys,
                                                                   const TKeys&   default_keys,
                                                                   QWidget*       parent) {
        const auto& properties          = config.getPrimaryButtonProperties();
        const auto& params              = config.getLayoutProperties().getPrimaryButtons();
        const auto& button_keys         = keys.getPrimaryButton();
        const auto& default_button_keys = default_keys.getPrimaryButton();

        // TODO If applied key is already used elsewhere, there will be confusion
        //      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
        //      primary button 4 has missing keybinding, both buttons will be assigned to Qt_Key4.
        //      There should be a validation system in place for all keybindings, for example a
        //      set with all keys which have already been exhausted.
        const auto key_getter = [&params, &button_keys,
                                 &default_button_keys](size_t i) -> keybindings {
                if (i < button_keys.size()) { return button_keys[i]; }
                if (i < default_button_keys.size()) {
                        qWarning()
                                << QString("Key for button %1 not found, applying default Qt_Key%1!")
                                           .arg(i + 1);
                        return default_button_keys[i];
                }

                qCritical() << "Number of buttons exceeds size of default keys! Buttons found:"
                            << params.size();
                return keybindings{Qt::Key_unknown};
        };

        std::vector<PrimaryButton*> buttons = {};
        buttons.reserve(params.size());

        for (size_t i = 0; i != params.size(); ++i) {
                PrimaryButton* button  = nullptr;
                QIcon          icon    = params[i].icon;
                QString        text    = params[i].text;
                QString        command = params[i].command;
                keybindings    keys    = key_getter(i);
                if constexpr (TApplet == applet::type::power_applet) {
                        auto type = params[i].type;

                        button = new TPrimaryButton(type, icon, text, keys, command, properties,
                                                    parent);
                } else {
                        button = new TPrimaryButton(icon, text, keys, command, properties, parent);
                }

                buttons.push_back(button);
        }

        if (buttons.empty()) { qFatal("No buttons found!"); }

        return buttons;
}
