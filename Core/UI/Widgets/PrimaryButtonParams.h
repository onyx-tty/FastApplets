// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/ButtonType.h"

#include <variant>
#include <QIcon>
#include <QString>

// Parameters required to construct a PrimaryButton.
struct PrimaryButtonParams final {
        ButtonType type = std::monostate{};
        QString    text;
        QString    command;
        QIcon      icon;
};
