// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/ButtonType.h"

#include <QIcon>
#include <QString>
#include <variant>

// Assign hardcoded icon associated with each button.
template<typename TPrimaryButtonType>
[[nodiscard]] QIcon iconFor(TPrimaryButtonType type);

// Assign hardcoded text associated with each button.
template<typename TPrimaryButtonType>
[[nodiscard]] QString textFor(TPrimaryButtonType type);

// Assign hardcoded command associated with each button.
template<typename TPrimaryButtonType>
[[nodiscard]] QString commandFor(TPrimaryButtonType type);

// Parameters required to construct a PrimaryButton.
struct PerPrimaryButtonParams final {
        button_type type = std::monostate();
        QString     text;
        QString     command;
        QIcon       icon;
};
