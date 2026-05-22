// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Types/Keybindings.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PrimaryButton.h"

#include <QObject>

class QBoxLayout;
class QIcon;
class QString;

// Power button with associated icon and keybindings.
//   - type: Determines which icon to use and which keybindings can activate it.
//   - command: The actual system action to execute (shutdown, reboot, etc.)
//   - keys: Key combinations that trigger this button.
class PowerButton final : public PrimaryButton {
        Q_OBJECT

private:
        const power_button_type type;
        const keybindings       keys;

public:
        explicit PowerButton(power_button_type type, const QIcon& icon, const QString& text,
                             const keybindings& keys, const QString& command);
        power_button_type  getType() const;
        const keybindings& getKeys() const;
};
