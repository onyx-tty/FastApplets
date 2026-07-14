// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButtonProperties.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <Qt>

namespace {

using PrimaryButton = config::schema::properties::PrimaryButton;

} // namespace

PrimaryButton::PrimaryButton(bool double_key_press, Qt::Alignment text_alignment,
                             Qt::Alignment icon_alignment, QSize icon_size, QSizePolicy policy) :
        double_key_press(double_key_press), text_alignment(text_alignment),
        icon_alignment(icon_alignment), icon_size(icon_size), policy(policy) {}

bool PrimaryButton::getDoubleKeyPress() const {
        return double_key_press;
}

const Qt::Alignment& PrimaryButton::getTextAlignment() const {
        return text_alignment;
}

const Qt::Alignment& PrimaryButton::getIconAlignment() const {
        return icon_alignment;
}

const QSize& PrimaryButton::getIconSize() const {
        return icon_size;
}

const QSizePolicy& PrimaryButton::getPolicy() const {
        return policy;
}
