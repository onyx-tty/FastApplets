// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/Types/Keybindings.h"
#include "UI/Types/ButtonID.h"

#include <vector>

class PowerButton;

struct PowerButtonRecord final {
        power_button_id    id;
        PowerButton*       widget;
        const keybindings  keys;
};

using PowerButtonRecords = std::vector<PowerButtonRecord>;
