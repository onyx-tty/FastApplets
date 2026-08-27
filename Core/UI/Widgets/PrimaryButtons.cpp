// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButtons.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Widgets/PrimaryButton.h"

#include <QDebug>
#include <QString>
#include <QWidget>
#include <Qt>
#include <QtGlobal>
#include <algorithm>
#include <cstddef>
#include <vector>

PrimaryButton* findPrimaryButton(int key, PrimaryButtons buttons) {
        const auto iter = std::find_if(
                buttons.cbegin(), buttons.cend(), [key](const PrimaryButton* button) -> bool {
                        if (!button) { return false; }
                        return button->getKeys().contains(key);
                });

        return iter != buttons.cend() ? *iter : nullptr;
}

PrimaryButtons makePrimaryButtons(const PrimaryButtonParams& params,
        const std::vector<keybindings>& keys, const std::vector<keybindings>& default_keys,
        QWidget* parent) {
        // TODO If applied key is already used elsewhere, the keys will behave unpredictably.
        //      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
        //      primary button 4 has missing keybinding, upon defaulting, primary button 4
        //      will be set to Qt_Key4 and both buttons will then be set to Qt_Key4.
        //      There should be a validation system in place for all keybindings, for example a
        //      set with all keys which have already been exhausted.
        const auto key_getter = [&params, &keys, &default_keys](size_t i) -> keybindings {
                if (i < keys.size()) { return keys[i]; }
                if (i < default_keys.size()) {
                        qWarning() << QString(
                                "Key for button %1 not found, applying default Qt_Key%1!")
                                              .arg(i + 1);
                        return default_keys[i];
                }

                qCritical() << "Number of buttons exceeds size of default keys! Buttons found:"
                            << params.per_button.size();
                return keybindings(Qt::Key_unknown);
        };

        PrimaryButtons buttons = {};
        buttons.reserve(params.per_button.size());

        for (size_t i = 0; i != params.per_button.size(); ++i) {
                button_type               type    = params.per_button[i].type;
                const QIcon&              icon    = params.per_button[i].icon;
                const QString&            text    = params.per_button[i].text;
                const QString&            command = params.per_button[i].command;
                const PrimaryButtonStyle& style   = params.style;
                keybindings               keys    = key_getter(i);
                auto* button = new PrimaryButton(type, icon, text, keys, command, style, parent);
                buttons.push_back(button);
        }

        if (buttons.empty()) { qFatal("No buttons found!"); }

        return buttons;
}
