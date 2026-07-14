// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

// Stores filepaths to config and keys.
namespace config {

struct Filepaths final {
        QString config;
        QString keys;
};

} // namespace config
