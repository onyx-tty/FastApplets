// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

class ConfigMapper;
class QIcon;
class QString;

// Assign hardcoded icons associated with each button.
// TODO: Auto-assign on parse instead of here
template<typename TPrimaryButtonType>
QIcon iconFor(TPrimaryButtonType type);

template<typename TPrimaryButtonType>
QString textFor(TPrimaryButtonType type);

template<typename TPrimaryButtonType>
QString commandFor(TPrimaryButtonType type);

// Stores properties describing the appearance of the layout.
template<typename TPrimaryButtonParams>
class LayoutProperties final {
private:
        friend class ConfigMapper;

        std::vector<TPrimaryButtonParams> primary_buttons;

public:
        explicit LayoutProperties(std::vector<TPrimaryButtonParams> primary_buttons = {});
        const std::vector<TPrimaryButtonParams>& getPrimaryButtons() const;
};

#include "LayoutProperties.tpp"
