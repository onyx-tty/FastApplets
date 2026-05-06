// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PrimaryButton.h"

#include <QBoxLayout>
#include <QIcon>

class QString;

class PowerButton final : public PrimaryButton {
        Q_OBJECT

private:
        const power_button_id id;
        const QString         dbus_action;

public:
        explicit PowerButton(power_button_id id, const QIcon& icon, const QString& text,
                             const QString& dbus_action, const QString& command);
        power_button_id getID() const;
        const QString&  getDBusAction() const;
};
