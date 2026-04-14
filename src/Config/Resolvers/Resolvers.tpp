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
#include "Config/TOML/Types/TomlArrayConditions.h"
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
                         bool force_override_on, const TomlArrayConditions& arr_conditions) {
        using DT = std::decay_t<T>;

        // Convert pointers to std::optional
        static auto normalize = [&](auto&& raw) -> std::optional<T> {
                using R = std::decay_t<decltype(raw)>;
                if constexpr (std::is_same_v<R, std::optional<DT>>) {
                        return raw;
                } else if constexpr (std::is_pointer_v<R>) {
                        return raw ? std::optional<DT>(*raw) : std::nullopt;
                } else {
                        static_assert(false, "Unknown extractor return type!");
                }
        };

        // Collapse extraction logic into that of a corresponding type
        static auto extract =
                [&](node_view node, const QString& path, bool is_override,
                    const TomlArrayConditions& arr_conditions = {}) -> std::optional<DT> {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        return normalize(extractor::table(node, path, is_override));
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        return normalize(extractor::array(node, path, is_override, arr_conditions));
                } else if constexpr (std::is_same_v<DT, QSize>) {
                        return normalize(extractor::qsize(node, path, is_override));
                } else if constexpr (std::is_same_v<DT, Qt::Alignment>) {
                        return normalize(extractor::alignment(node, path, is_override));
                } else if constexpr (std::is_same_v<DT, QSizePolicy>) {
                        return normalize(extractor::size_policy(node, path, is_override));
                } else if constexpr (std::is_same_v<DT, QString>) {
                        return normalize(extractor::qstring(node, path, is_override));
                } else {
                        return normalize(extractor::value<DT>(node, path, is_override));
                }
        };

        // Validate and attempt extraction of each passed source, prioritizing earliest ones
        for (size_t i = 0; i != sources.size(); ++i) {
                auto& source = *(sources.begin() + i);

                // If 'i' is not the last index then 'source' is an override
                bool is_source_override = (i < sources.size() - 1) || force_override_on ? true
                                                                                        : false;

                if (auto result = extract(source.node, makeCfgPath(source.scope, path_context),
                                          is_source_override, arr_conditions)) {
                        return *result;
                }
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}

// Use to skip validation of return value and to automatically default
// On success: extract from a node
// On failure: copy default value
template<typename T, typename TDefault>
T resolveOr(std::initializer_list<Source> sources, const TDefault& defaults,
            const QString& path_context, const TomlArrayConditions& arr_conditions) {
        return resolve<T>(sources, path_context, false, arr_conditions).value_or(defaults);
}

// Use to try and extract a value from a node into a specific attribute, and if that fails, to
// default a completely different object
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: write result into a provided attribute
// On failure: overwrite object with object_defaults entirely
template<typename TAttribute, typename TObject>
void resolveOrDefault(std::initializer_list<Source> sources, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, const QString& path_context,
                      const TomlArrayConditions& arr_conditions) {
        if (auto result = resolve<TAttribute>(sources, path_context, false, arr_conditions)) {
                attribute = result.value();
        } else {
                object = object_defaults;
        }
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
        if (auto result = resolve<TRaw>(sources, path_context, false)) {
                attribute = transform(std::move(result.value()));
        } else {
                object = object_defaults;
        }
}
