// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/Enums/ButtonIDs.h"

#include <QIcon>
#include <QString>

// Parameters required to construct a PowerButton
struct PowerButtonParams final {
        power_button_id id;
        QString         text;
        long            order;
        QString         command;
        QIcon           icon;
};
