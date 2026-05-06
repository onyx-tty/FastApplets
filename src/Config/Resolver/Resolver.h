// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/Types/Source.h"
#include "Config/Types/NodeView.h"
#include "TomlQt/ArrayBounds.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <toml++/toml.hpp>
#include <QString>
#include <Qt>

class QSizePolicy;
class QSize;
class PathContext;

class Resolver final {
public:
        Resolver() = delete;

        // Use if return value and defaulting must be handled manually
        // On success: extract from a node, return as std::optional<T>
        // On failure: return std::nullopt
        template<typename T>
        static std::optional<T> from(std::initializer_list<Source> sources,
                                     const PathContext&            path_context,
                                     const tomlqt::ArrayBounds&    arr_bounds = {},
                                     const QString&                arr_format = {});

        // Use to skip validation of return value and to automatically default
        // On success: extract from a node
        // On failure: copy default value
        template<typename T, typename TDefault>
        static T fromOr(std::initializer_list<Source> sources, const TDefault& defaults,
                        const PathContext& path_context, const tomlqt::ArrayBounds& arr_bounds = {},
                        const QString& arr_format = {});

        // Use to try and extract a value from a node into a specific attribute, and if that fails, to
        // default a completely different object
        // For example: if button ID is erroneous, default the button itself, not just the id
        //
        // On success: write result into a provided attribute
        // On failure: overwrite object with object_defaults entirely
        template<typename TAttribute, typename TObject>
        static void fromOrDefault(std::initializer_list<Source> sources, TAttribute& attribute,
                                  TObject& object, const TObject& object_defaults,
                                  const PathContext&         path_context,
                                  const tomlqt::ArrayBounds& arr_bounds = {},
                                  const QString&             arr_format = {});

        // Use if fromOrDefault is the optimal choice, but the extracted value must first be transformed
        // before being put into use
        // For example: if button ID is erroneous, default the button itself, not just the id
        //
        // On success: transform, then write result into an attribute
        // On failure: overwrite the object with object_defaults entirely
        template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
        static void fromTransformOrDefault(std::initializer_list<Source> sources,
                                           TAttribute& attribute, TObject& object,
                                           const TObject& object_defaults, Transform&& transform,
                                           const PathContext& path_context);
};

#include "Resolver.tpp"
