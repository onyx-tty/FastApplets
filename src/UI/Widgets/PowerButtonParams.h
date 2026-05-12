// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/Types/ButtonType.h"

#include <QIcon>
#include <QString>

// Parameters required to construct a PowerButton.
// Currently id, text, order, command, come from config, icon is assigned from id.
struct PowerButtonParams final {
        power_button_type id;
        QString           text;
        long              order;
        QString           command;
        QIcon             icon;
};
