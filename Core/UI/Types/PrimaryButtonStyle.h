// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSize>
#include <QSizePolicy>
#include <Qt>
#include <toml++/toml.hpp>

// TODO: Add more comprehensive docs
// Stores data used to style each PrimaryButton.
struct PrimaryButtonStyle final {
        Qt::Alignment text_alignment;
        Qt::Alignment icon_alignment;
        QSize         icon_size;
        QSizePolicy   policy;
};
