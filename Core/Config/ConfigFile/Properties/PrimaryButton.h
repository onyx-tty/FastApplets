// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <Qt>

namespace config {
class ConfigMapper;
} // namespace config

namespace config::schema::properties {

// Stores data used to style each PrimaryButton.
class PrimaryButton final {
private:
        friend class config::ConfigMapper;

        bool          double_key_press;
        Qt::Alignment text_alignment;
        Qt::Alignment icon_alignment;
        QSize         icon_size;
        QSizePolicy   policy;

public:
        explicit PrimaryButton(bool double_key_press = {}, Qt::Alignment text_alignment = {},
                               Qt::Alignment icon_alignment = {}, QSize icon_size = {},
                               QSizePolicy policy = {});
        [[nodiscard]] bool                 getDoubleKeyPress() const;
        [[nodiscard]] const Qt::Alignment& getTextAlignment() const;
        [[nodiscard]] const Qt::Alignment& getIconAlignment() const;
        [[nodiscard]] const QSize&         getIconSize() const;
        [[nodiscard]] const QSizePolicy&   getPolicy() const;
};

} // namespace config::schema::properties
