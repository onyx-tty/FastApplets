// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <Qt>

// Stores data used to style each PrimaryButton.
class PrimaryButtonProperties final {
private:
        friend class ConfigMapper;

        bool          double_key_press;
        Qt::Alignment text_alignment;
        Qt::Alignment icon_alignment;
        QSize         icon_size;
        QSizePolicy   policy;

public:
        explicit PrimaryButtonProperties(bool          double_key_press = {},
                                         Qt::Alignment text_alignment   = {},
                                         Qt::Alignment icon_alignment = {}, QSize icon_size = {},
                                         QSizePolicy policy = {});
        bool                 getDoubleKeyPress() const;
        const Qt::Alignment& getTextAlignment() const;
        const Qt::Alignment& getIconAlignment() const;
        const QSize&         getIconSize() const;
        const QSizePolicy&   getPolicy() const;
};
