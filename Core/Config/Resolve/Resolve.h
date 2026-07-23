// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "TomlQt/ArrayBounds.h"

#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QStringView>
#include <Qt>

namespace config::resolve {
class Candidates;
class PathContext;
} // namespace config::resolve

class QSize;
class QSizePolicy;
class QString;

// Extracts typed values from TOML nodes with fallback chains and error handling.
//
// All methods accept multiple config::resolve::Candidates (priority ordered). The first candidate that
// provides a valid value wins. Invalid values (incorrect type, bound failures)
// are treated as missing.
//
// Extraction rules by type:
//   - toml::table/array: direct node resolution
//   - QSize, Qt::Alignment, QSizePolicy, QStringView: TomlQt converters
//   - Other types: toml++ native value extraction
//
// Quick reference:
// - from()          -> returns optional<T>, manual error handling
// - fromAs()        -> returns T*, manual error handling
// - fromOrDefault() -> sets attribute OR overwrites entire object
// - fromTransformOrDefault() -> sets transformed attribute OR overwrites entire object
namespace config::resolve {

// Pure extraction with no side effects.
//
// Requires a manual std::nullopt check.
//
// On success: returns std::optional<T>
// On failure: returns std::nullopt
template<typename T>
requires(!std::is_same_v<T, toml::table> && !std::is_same_v<T, toml::array>)
[[nodiscard]] std::optional<T> from(const Candidates& candidates, const PathContext& path_context);

// Pure extraction with no side effects.
//
// Requires a manual nullptr check.
//
// On success: returns T*
// On failure: returns nullptr
template<typename T>
requires(!std::is_same_v<std::decay_t<T>, QSize> && !std::is_same_v<std::decay_t<T>, Qt::Alignment>
         && !std::is_same_v<std::decay_t<T>, QSizePolicy>
         && !std::is_same_v<std::decay_t<T>, QString>)
[[nodiscard]] const T* fromAs(const Candidates& candidates, const PathContext& path_context,
                              const tomlqt::ArrayBounds& arr_bounds = {},
                              QStringView                arr_format = {});

// Extraction that can fall back to replacing the entire parent object.
//
// Automatically defaults the parent if the passed attribute is important.
//
// On success: writes extracted value to attribute
// On failure: silently replaces entire object with object_defaults, with
//             no partial state
template<typename TAttribute, typename TObject>
void fromOrDefault(const Candidates& candidates, TAttribute& attribute, TObject& object,
                   const TObject& object_defaults, const PathContext& path_context,
                   const tomlqt::ArrayBounds& arr_bounds = {}, QStringView arr_format = {});

// Like fromOrDefault but with a transformation step before attribute assignment.
//
// Automatically defaults the parent if the passed attribute is important.
// Additionally, transforms the extracted value to adapt it to the attribute.
//
// On success: writes extracted and transformed value to attribute
// On failure: silently replaces entire object with object_defaults, with
//             no partial state
template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
void fromTransformOrDefault(const Candidates& candidates, TAttribute& attribute, TObject& object,
                            const TObject& object_defaults, Transform transform,
                            const PathContext& path_context);

} // namespace config::resolve

#include "Resolve.tpp"
