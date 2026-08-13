// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/ArraySpec.h"

#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QStringView>
#include <Qt>

namespace config::select {
class Candidates;
} // namespace config::select

// Extracts typed values from TOML nodes with fallback chains and error handling.
//
// All methods accept multiple config::select::Candidates (priority ordered). The first candidate that
// provides a valid value wins. Invalid values (incorrect type, bound failures)
// are treated as missing.
//
// Extraction rules by type:
//   - toml::table/array: direct node resolution
//   - QSize, Qt::Alignment, QSizePolicy, QStringView: TomlQt converters
//   - Other types: toml++ native value extraction
//
// Quick reference:
// - from() -> returns optional<T>, manual error handling
// - from() -> returns T*, manual error handling
namespace config::resolve {

using config::ArraySpec;
using config::select::Candidates;

// Pure extraction with no side effects.
//
// Requires a manual std::nullopt check.
//
// On success: returns std::optional<T>
// On failure: returns std::nullopt
template<typename T>
requires(!std::is_same_v<T, toml::table> && !std::is_same_v<T, toml::array>)
[[nodiscard]] std::optional<T> from(const Candidates& candidates);

// Pure extraction with no side effects.
//
// Requires a manual nullptr check.
//
// On success: returns const T*
// On failure: returns nullptr
template<typename T>
requires(std::is_same_v<std::decay_t<T>, toml::table>
         || std::is_same_v<std::decay_t<T>, toml::array>)
[[nodiscard]] const T* from(const Candidates& candidates, const ArraySpec& array_spec = {});

} // namespace config::resolve

#include "Resolve.tpp"
