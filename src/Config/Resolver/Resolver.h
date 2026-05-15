// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "TomlQt/ArrayBounds.h"

#include <initializer_list>
#include <optional>
#include <toml++/toml.hpp>
#include <QString>
#include <Qt>

class PathContext;
class Source;
class QSizePolicy;
class QSize;

// Extracts typed values from TOML nodes with fallback chains and error handling.
//
// All methods accept multiple Sources (priority ordered). The first source that
// provides a valid value wins. Invalid values (incorrect type, bound failures)
// are treated as missing.
//
// Extraction rules by type:
//   - toml::table/array: direct node resolution
//   - QSize, Qt::Alignment, QSizePolicy, QString: TomlQt converters
//   - Other types: toml++ native value extraction
//
// Quick reference:
// - from()          -> returns optional<T>, manual error handling
// - fromOr()        -> returns T, either extracted or defaults, no side effects
// - fromOrDefault() -> sets attribute OR overwrites entire object
// - fromTransformOrDefault() -> sets transformed attribute OR overwrites entire object
class Resolver final {
public:
        Resolver() = delete;

        // Pure extraction with no side effects.
        //
        // Requires a manual std::nullopt check.
        //
        // On success: returns optional extracted value
        // On failure: returns std::nullopt
        template<typename T>
        static std::optional<T> from(std::initializer_list<Source> sources,
                                     const PathContext&            path_context,
                                     const tomlqt::ArrayBounds&    arr_bounds = {},
                                     const QString&                arr_format = {});

        // Extraction with automatic default value.
        //
        // Always returns T, automatically handles null checks.
        //
        // On success: returns raw extracted value
        // On failure: returns defaults
        template<typename T, typename TDefault>
        static T fromOr(std::initializer_list<Source> sources, const TDefault& defaults,
                        const PathContext& path_context, const tomlqt::ArrayBounds& arr_bounds = {},
                        const QString& arr_format = {});

        // Extraction that can fall back to replacing the entire parent object.
        //
        // Automatically defaults the parent if the passed attribute is important.
        //
        // On success: writes extracted value to attribute
        // On failure: silently replaces entire object with object_defaults, with
        //             no partial state
        template<typename TAttribute, typename TObject>
        static void fromOrDefault(std::initializer_list<Source> sources, TAttribute& attribute,
                                  TObject& object, const TObject& object_defaults,
                                  const PathContext&         path_context,
                                  const tomlqt::ArrayBounds& arr_bounds = {},
                                  const QString&             arr_format = {});

        // Like fromOrDefault but with a transformation step before attribute assignment.
        //
        // Automatically defaults the parent if the passed attribute is important.
        // Additionally, transforms the extracted value to adapt it to the attribute.
        //
        // On success: writes extracted and transformed value to attribute
        // On failure: silently replaces entire object with object_defaults, with
        //             no partial state
        template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
        static void fromTransformOrDefault(std::initializer_list<Source> sources,
                                           TAttribute& attribute, TObject& object,
                                           const TObject& object_defaults, Transform&& transform,
                                           const PathContext& path_context);
};

#include "Resolver.tpp"
