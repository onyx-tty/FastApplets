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
#include "Resolvers.h"

#include <optional>
#include <qnamespace.h>
#include <string_view>
#include <toml++/toml.hpp>

class QSizePolicy;
class QSize;

// Use if return value and defaulting must be handled manually
// On success: extract from a node, return as std::optional<T>
// On failure: return std::nullopt
// Forcefully mark as an override by passing 'force_override_on = true',
// useful for single sources, as only multiple sources get automatic
// detection of overrides
template<typename T>
std::optional<T> resolve(std::initializer_list<Source> sources, const QString& path_context,
                         bool force_override_on) {
        using DT = std::decay_t<T>;

        // Collapse extraction logic into that of a corresponding type
        static auto extract = [&](node_view node, const QString& path,
                                  bool is_override) -> std::optional<DT> {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        return extractor::table(node, path, is_override);
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        return extractor::array(node, path, is_override);
                } else if constexpr (std::is_same_v<DT, QSize>) {
                        return extractor::qsize(node, path, is_override);
                } else if constexpr (std::is_same_v<DT, Qt::Alignment>) {
                        return extractor::alignment(node, path, is_override);
                } else if constexpr (std::is_same_v<DT, QSizePolicy>) {
                        return extractor::size_policy(node, path, is_override);
                } else if constexpr (std::is_same_v<DT, QString>) {
                        return extractor::qstring(node, path, is_override);
                } else {
                        return extractor::value<DT>(node, path, is_override);
                }
        };

        // Validate and attempt extraction of each passed source, prioritizing earliest ones
        for (size_t i = 0; i != sources.size(); ++i) {
                auto& source = *(sources.begin() + i);

                // If 'i' is not the last index then 'source' is an override
                bool is_source_override = (i < sources.size() - 1) || force_override_on ? true
                                                                                        : false;

                if (auto result = extract(source.node, makeCfgPath(source.scope, path_context),
                                          is_source_override)) {
                        return *result;
                }
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}

template<typename T, typename... Sources>
requires(std::is_convertible_v<Sources, Source> && ...)
std::optional<T> resolve(const QString& path_context, bool force_override_on, Sources&&... sources) {
        return resolve<T>({std::forward<Sources>(sources)...}, path_context, force_override_on);
}

// Use to skip validation of return value and to automatically default
// On success: extract from a node
// On failure: copy default value
template<typename T, typename DefaultT>
T resolveOr(std::initializer_list<Source> sources, const DefaultT& defaults,
            const QString& path_context) {
        return resolve<T>(sources, path_context).value_or(defaults);
}

template<typename T, typename DefaultT, typename... Sources>
requires(std::is_convertible_v<Sources, Source> && ...)
T resolveOr(const QString& path_context, const DefaultT& defaults, Sources&&... sources) {
        return resolveOr<T>({std::forward<Sources>(sources)...}, defaults, path_context);
}

// Use to try and extract a value from a node into a specific attribute, and if that fails, to
// default a completely different object
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: write result into a provided attribute
// On failure: overwrite object with object_defaults entirely
template<typename TAttribute, typename TObject>
void resolveOrDefault(std::initializer_list<Source> sources, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, const QString& path_context) {
        if (auto result = resolve<TAttribute>(sources, path_context)) {
                attribute = result.value();
        } else {
                object = object_defaults;
        }
}

template<typename TAttribute, typename TObject, typename... Sources>
requires(std::is_convertible_v<Sources, Source> && ...)
void resolveOrDefault(const QString& path_context, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, Sources&&... sources) {
        resolveOrDefault<TAttribute, TObject>({std::forward<Sources>(sources)...}, attribute,
                                              object, object_defaults, path_context);
}

// Use if resolveOrDefault is the optimal choice, but the extracted value must first be transformed
// before being put into use
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: transform, then write result into an attribute
// On failure: overwrite the object with object_defaults entirely
template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
void resolveTransformOrDefault(std::initializer_list<Source> sources, TAttribute& attribute,
                               TObject& object, const TObject& object_defaults,
                               Transform&& transform, const QString& path_context) {
        if (auto result = resolve<TRaw>(sources, path_context)) {
                attribute = transform(std::move(result.value()));
        } else {
                object = object_defaults;
        }
}

template<typename TRaw, typename TAttribute, typename TObject, typename Transform, typename... Sources>
void resolveTransformOrDefault(const QString& path_context, TAttribute& attribute, TObject& object,
                               const TObject& object_defaults, Transform&& transform,
                               Sources&&... sources) {
        resolveTransformOrDefault<TRaw>({std::forward<Sources>(sources)...}, attribute, object,
                                        object_defaults, transform, path_context);
}
