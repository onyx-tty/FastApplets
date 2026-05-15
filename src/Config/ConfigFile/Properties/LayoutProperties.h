// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <vector>

class ConfigMapper;
class QIcon;
class QString;

// Assign hardcoded icons associated with each button.
// TODO: Auto-assign on parse
QIcon iconFor(power_button_type id);

// Stores properties describing the appearance of the layout.
class LayoutProperties final {
private:
        friend class ConfigMapper;

        std::vector<PowerButtonParams> power_buttons;

public:
        explicit LayoutProperties(std::vector<PowerButtonParams> power_buttons = {});
        const std::vector<PowerButtonParams>& getPowerButtons() const;
};
