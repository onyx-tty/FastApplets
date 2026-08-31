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
        const std::vector<keybindings>& keys_vec, QWidget* parent) {
        PrimaryButtons buttons = {};
        buttons.reserve(params.per_button.size());

        // Log a warning and substitute a keybindings set with Qt::Key_unknown for missing keys.
        const auto keys_getter = [&keys_vec](size_t i) -> const keybindings& {
                if (i > keys_vec.size() || keys_vec[i] == keybindings(Qt::Key_unknown)) {
                        qWarning() << "No key found";
                        static keybindings unknown = {Qt::Key_unknown};
                        return unknown;
                }

                return keys_vec[i];
        };

        for (size_t i = 0; i != params.per_button.size(); ++i) {
                button_type               type    = params.per_button[i].type;
                const QIcon&              icon    = params.per_button[i].icon;
                const QString&            text    = params.per_button[i].text;
                const QString&            command = params.per_button[i].command;
                const PrimaryButtonStyle& style   = params.style;
                const keybindings&        keys    = keys_getter(i);

                auto* button = new PrimaryButton(type, icon, text, keys, command, style, parent);
                buttons.push_back(button);
        }

        if (buttons.empty()) { qFatal("No buttons found!"); }

        return buttons;
}
