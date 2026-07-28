// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <Qt>

// Stores data used to style each PrimaryButton.
struct PrimaryButtonStyle final {
        Qt::Alignment text_alignment;
        Qt::Alignment icon_alignment;
        QSize         icon_size;
        QSizePolicy   policy;
        bool          double_key_press;
};
