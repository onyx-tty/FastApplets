// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

// 'none' represents empty state, e.g. no button selected, if used for that purpose
enum class power_button_type { none, shutdown, reboot, suspend, hibernate };

template<typename TPrimaryButtonType>
TPrimaryButtonType toPrimaryButtonType(const QString& string);
