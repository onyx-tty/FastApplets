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
class ResolverCandidate;
class QSizePolicy;
class QSize;

// Extracts typed values from TOML nodes with fallback chains and error handling.
//
// All methods accept multiple ResolverCandidates (priority ordered). The first candidate that
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
        static std::optional<T> from(ResolverCandidates candidates, const PathContext& path_context,
                                     const tomlqt::ArrayBounds& arr_bounds = {},
                                     const QString&             arr_format = {});

        // Extraction that can fall back to replacing the entire parent object.
        //
        // Automatically defaults the parent if the passed attribute is important.
        //
        // On success: writes extracted value to attribute
        // On failure: silently replaces entire object with object_defaults, with
        //             no partial state
        template<typename TAttribute, typename TObject>
        static void fromOrDefault(ResolverCandidates candidates, TAttribute& attribute,
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
        static void fromTransformOrDefault(ResolverCandidates candidates, TAttribute& attribute,
                                           TObject& object, const TObject& object_defaults,
                                           Transform&& transform, const PathContext& path_context);
};

#include "Resolver.tpp"
