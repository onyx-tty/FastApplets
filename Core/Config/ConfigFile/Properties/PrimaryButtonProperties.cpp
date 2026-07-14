// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButtonProperties.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <Qt>

config::schema::properties::PrimaryButton::PrimaryButton(bool          double_key_press,
                                                         Qt::Alignment text_alignment,
                                                         Qt::Alignment icon_alignment,
                                                         QSize icon_size, QSizePolicy policy) :
        double_key_press(double_key_press), text_alignment(text_alignment),
        icon_alignment(icon_alignment), icon_size(icon_size), policy(policy) {}

bool config::schema::properties::PrimaryButton::getDoubleKeyPress() const {
        return double_key_press;
}

const Qt::Alignment& config::schema::properties::PrimaryButton::getTextAlignment() const {
        return text_alignment;
}

const Qt::Alignment& config::schema::properties::PrimaryButton::getIconAlignment() const {
        return icon_alignment;
}

const QSize& config::schema::properties::PrimaryButton::getIconSize() const {
        return icon_size;
}

const QSizePolicy& config::schema::properties::PrimaryButton::getPolicy() const {
        return policy;
}
