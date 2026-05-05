// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/Widgets/PowerButtonParams.h"

#include <vector>

class ConfigMapper;
class QIcon;
class QString;

QIcon iconFor(power_button_id id);

QString dbusMethodFor(power_button_id id);

class LayoutProperties final {
private:
        friend class ConfigMapper;

        std::vector<PowerButtonParams> power_buttons;

public:
        explicit LayoutProperties(std::vector<PowerButtonParams> power_buttons = {});
        const std::vector<PowerButtonParams>& getPowerButtons() const;
};
