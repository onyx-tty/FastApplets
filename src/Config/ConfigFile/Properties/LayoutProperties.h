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
QIcon iconFor(power_button_type type);

QString textFor(power_button_type type);

QString commandFor(power_button_type type);

// Stores properties describing the appearance of the layout.
// TODO: Template overloads
class LayoutProperties final {
private:
        friend class ConfigMapper;

        // TODO: rename to primary_buttons + template overload
        std::vector<PowerButtonParams> power_buttons;

public:
        // TODO: rename to primary_buttons + template overload, for both
        explicit LayoutProperties(std::vector<PowerButtonParams> power_buttons = {});
        const std::vector<PowerButtonParams>& getPowerButtons() const;
};
