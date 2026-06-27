// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButtonsFactory.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButton.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <cstddef>
#include <vector>
#include <QDebug>
#include <QIcon>
#include <QString>
#include <Qt>
#include <QtGlobal>

std::vector<PrimaryButton*> PrimaryButtonsFactory::create(
        const std::vector<PrimaryButtonParams>& params, const PrimaryButtonProperties& properties,
        const std::vector<keybindings>& keys, const std::vector<keybindings>& default_keys,
        QWidget* parent) {
        // TODO If applied key is already used elsewhere, the keybindings will be unpredictable.
        //      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
        //      primary button 4 has missing keybinding, both buttons will be assigned to Qt_Key4.
        //      There should be a validation system in place for all keybindings, for example a
        //      set with all keys which have already been exhausted.
        const auto key_getter = [&params, &keys, &default_keys](size_t i) -> keybindings {
                if (i < keys.size()) { return keys[i]; }
                if (i < default_keys.size()) {
                        qWarning()
                                << QString("Key for button %1 not found, applying default Qt_Key%1!")
                                           .arg(i + 1);
                        return default_keys[i];
                }

                qCritical() << "Number of buttons exceeds size of default keys! Buttons found:"
                            << params.size();
                return keybindings{Qt::Key_unknown};
        };

        std::vector<PrimaryButton*> buttons = {};
        buttons.reserve(params.size());

        for (size_t i = 0; i != params.size(); ++i) {
                ButtonType  type    = params[i].type;
                QIcon       icon    = params[i].icon;
                QString     text    = params[i].text;
                QString     command = params[i].command;
                keybindings keys    = key_getter(i);
                auto*       button  = new PrimaryButton(type, icon, text, keys, command, properties,
                                                        parent);
                buttons.push_back(button);
        }

        if (buttons.empty()) { qFatal("No buttons found!"); }

        return buttons;
}
