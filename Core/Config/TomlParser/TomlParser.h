// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>
#include <toml++/toml.hpp>

// Parses TOML format.
//
// This is a low-level file parser used to obtain toml::tables for mapping.
class TomlParser final {
public:
        TomlParser() = delete;

        // Parses a single table at given filepath.
        // If file exists and is parsed successfully, returns a parsed table.
        // Otherwise returns an empty table and logs warnings.
        static toml::table file(std::string_view filepath);
};
