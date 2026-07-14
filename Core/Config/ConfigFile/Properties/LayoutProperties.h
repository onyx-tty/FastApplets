// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <vector>

namespace config {
class ConfigMapper;
} // namespace config

class QIcon;
class QString;

// Assign hardcoded icon associated with each button.
template<typename TPrimaryButtonType>
QIcon iconFor(TPrimaryButtonType type);

// Assign hardcoded text associated with each button.
template<typename TPrimaryButtonType>
QString textFor(TPrimaryButtonType type);

// Assign hardcoded command associated with each button.
template<typename TPrimaryButtonType>
QString commandFor(TPrimaryButtonType type);

namespace config::schema::properties {

// Stores properties describing the appearance of the layout.
class Layout final {
private:
        friend class config::ConfigMapper;

        std::vector<PrimaryButtonParams> primary_buttons;

public:
        explicit Layout(std::vector<PrimaryButtonParams> primary_buttons = {});
        const std::vector<PrimaryButtonParams>& getPrimaryButtons() const;
};

} // namespace config::schema::properties
