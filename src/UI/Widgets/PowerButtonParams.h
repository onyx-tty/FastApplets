// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/Widgets/PrimaryButtonParams.h"

#include <utility>
#include <QIcon>
#include <QString>

struct PowerButtonParams final : PrimaryButtonParams {
        power_button_id id;
        QString         command;
        QString         dbus_method;
        QIcon           icon;

        PowerButtonParams(power_button_id id = power_button_id::none, QString text = {},
                          long order = {}, QString command = {}, QString dbus_method = {},
                          const QIcon& icon = {}) :
                PrimaryButtonParams(std::move(text), order), id(id), command(std::move(command)),
                dbus_method(std::move(dbus_method)), icon(icon) {}
};
