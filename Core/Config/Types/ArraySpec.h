// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QStringView>
#include <TomlQt/ArrayBounds.h>
#include <optional>

namespace config {

using tomlqt::ArrayBounds;

struct ArraySpec final {
        std::optional<ArrayBounds> bounds;
        QStringView                format;
};

} // namespace config
