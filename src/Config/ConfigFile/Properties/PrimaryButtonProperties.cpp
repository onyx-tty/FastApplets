// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButtonProperties.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <Qt>

PrimaryButtonProperties::PrimaryButtonProperties(Qt::Alignment text_alignment,
                                                 Qt::Alignment icon_alignment, QSize icon_size,
                                                 QSizePolicy policy) :
        text_alignment(text_alignment), icon_alignment(icon_alignment), icon_size(icon_size),
        policy(policy) {}

const Qt::Alignment& PrimaryButtonProperties::getTextAlignment() const {
        return text_alignment;
}

const Qt::Alignment& PrimaryButtonProperties::getIconAlignment() const {
        return icon_alignment;
}

const QSize& PrimaryButtonProperties::getIconSize() const {
        return icon_size;
}

const QSizePolicy& PrimaryButtonProperties::getPolicy() const {
        return policy;
}
