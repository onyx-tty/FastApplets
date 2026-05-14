// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <toml++/toml.hpp>

// Parses TOML format.
//
// This is a low-level file parser used to obtain toml::tables for mapping.
class TomlParser final {
private:
        // Parses a single table at given filepath.
        // If file exists and is parsed successfully, returns a parsed table.
        // Otherwise returns an empty table and logs warnings.
        static toml::table createTable(const std::string& filepath);

public:
        TomlParser() = delete;

        // Alias for createTable
        // TODO: Move createTable logic here, this file used to forward one of
        //       the 4 configs to createTable, but now it's no longer needed
        //       since TomlParser is a standalone now.
        static constexpr auto parseFile = createTable;
};
