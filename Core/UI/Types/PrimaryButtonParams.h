// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"

#include <vector>

// Parameters required to construct a PrimaryButton.
struct PrimaryButtonParams final {
        std::vector<PerPrimaryButtonParams> per_button;
        PrimaryButtonStyle                  style;
        PrimaryButtonBehavior               behavior;
};
