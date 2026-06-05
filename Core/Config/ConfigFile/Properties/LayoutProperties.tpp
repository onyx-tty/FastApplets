// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "LayoutProperties.h"

#include <utility>
#include <vector>
#include <QIcon>
#include <QResource>
#include <QString>

template<typename TPrimaryButtonParams>
LayoutProperties<TPrimaryButtonParams>::LayoutProperties(
        std::vector<TPrimaryButtonParams> primary_buttons) :
        primary_buttons(std::move(primary_buttons)) {}

template<typename TPrimaryButtonParams>
const std::vector<TPrimaryButtonParams>& LayoutProperties<TPrimaryButtonParams>::getPrimaryButtons()
        const {
        return primary_buttons;
}
