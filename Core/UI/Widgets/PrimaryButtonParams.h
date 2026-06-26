// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/ButtonType.h"

#include <optional>
#include <QIcon>
#include <QString>

// Parameters required to construct a PrimaryButton.
struct PrimaryButtonParams final {
        ButtonType type = std::nullopt;
        QString    text;
        QString    command;
        QIcon      icon;
};
