// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/PrimaryButtonParams.h"

#include <vector>

class QIcon;
class QString;

// Assign hardcoded icon associated with each button.
template<typename TPrimaryButtonType>
[[nodiscard]] QIcon iconFor(TPrimaryButtonType type);

// Assign hardcoded text associated with each button.
template<typename TPrimaryButtonType>
[[nodiscard]] QString textFor(TPrimaryButtonType type);

// Assign hardcoded command associated with each button.
template<typename TPrimaryButtonType>
[[nodiscard]] QString commandFor(TPrimaryButtonType type);

// Stores properties describing the appearance of the layout.
struct LayoutProperties final {
        std::vector<PrimaryButtonParams> primary_buttons;
};
