// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <vector>

class ConfigMapper;
class QIcon;
class QString;

// Assign hardcoded icons associated with each button.
template<typename TPrimaryButtonType>
QIcon iconFor(TPrimaryButtonType type);

// Assign hardcoded text associated with each button.
template<typename TPrimaryButtonType>
QString textFor(TPrimaryButtonType type);

// Assign hardcoded command associated with each button.
template<typename TPrimaryButtonType>
QString commandFor(TPrimaryButtonType type);

// Stores properties describing the appearance of the layout.
class LayoutProperties final {
private:
        friend class ConfigMapper;

        std::vector<PrimaryButtonParams> primary_buttons;

public:
        explicit LayoutProperties(std::vector<PrimaryButtonParams> primary_buttons = {});
        const std::vector<PrimaryButtonParams>& getPrimaryButtons() const;
};
