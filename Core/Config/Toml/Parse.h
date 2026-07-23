// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <toml++/toml.hpp>

class QString;

namespace config {

// Parses a single TOML table at given filepath.
//
// If file exists and is parsed successfully, returns a parsed table.
// Otherwise returns an empty table and logs warnings.
//
// This is a low-level file parser used to obtain toml::tables for mapping.
[[nodiscard]] toml::table parseTomlFile(const QString& filepath);

} // namespace config
