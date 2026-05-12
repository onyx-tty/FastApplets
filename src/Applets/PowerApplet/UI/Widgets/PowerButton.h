// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Types/Keybindings.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PrimaryButton.h"

#include <QBoxLayout>
#include <QIcon>

class QString;

class PowerButton final : public PrimaryButton {
        Q_OBJECT

private:
        const power_button_type id;
        const keybindings       keys;

public:
        explicit PowerButton(power_button_type id, const QIcon& icon, const QString& text,
                             const keybindings& keys, const QString& command);
        power_button_type  getID() const;
        const keybindings& getKeys() const;
};
