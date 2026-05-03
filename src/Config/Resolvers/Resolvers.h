/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "Config/TOML/Types/NodeView.h"
#include "TomlQt/ArrayBounds.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <toml++/toml.hpp>
#include <QString>
#include <QStringView>
#include <Qt>

class QSizePolicy;
class QSize;

struct Source final {
        node_view        node;
        std::string_view scope;
        bool             quiet = false;
};

// TODO Move elsewhere
class PathContext final {
private:
        QString path_context;
        QString filename;
        char    separator;

public:
        explicit PathContext(QStringView filename, QStringView path_context, char separator = '.');
        QString     makePath(std::string_view scope) const;
        PathContext getExtended(std::string_view segment) const;
        PathContext getExtended(size_t index) const;
};

// Use if return value and defaulting must be handled manually
// On success: extract from a node, return as std::optional<T>
// On failure: return std::nullopt
template<typename T>
std::optional<T> resolve(std::initializer_list<Source> sources, const PathContext& path_context,
                         const tomlqt::ArrayBounds& arr_bounds = {},
                         const QString&             arr_format = {});

// Use to skip validation of return value and to automatically default
// On success: extract from a node
// On failure: copy default value
template<typename T, typename TDefault>
T resolveOr(std::initializer_list<Source> sources, const TDefault& defaults,
            const PathContext& path_context, const tomlqt::ArrayBounds& arr_bounds = {},
            const QString& arr_format = {});

// Use to try and extract a value from a node into a specific attribute, and if that fails, to
// default a completely different object
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: write result into a provided attribute
// On failure: overwrite object with object_defaults entirely
template<typename TAttribute, typename TObject>
void resolveOrDefault(std::initializer_list<Source> sources, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, const PathContext& path_context,
                      const tomlqt::ArrayBounds& arr_bounds = {}, const QString& arr_format = {});

// Use if resolveOrDefault is the optimal choice, but the extracted value must first be transformed
// before being put into use
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: transform, then write result into an attribute
// On failure: overwrite the object with object_defaults entirely
template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
void resolveTransformOrDefault(std::initializer_list<Source> sources, TAttribute& attribute,
                               TObject& object, const TObject& object_defaults,
                               Transform&& transform, const PathContext& path_context);

#include "Resolvers.tpp"
