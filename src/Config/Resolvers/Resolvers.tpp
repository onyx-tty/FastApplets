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

#include "Config/Types/NodeView.h"
#include "CppUtils/Log/QtLog.h"
#include "Resolvers.h"
#include "TomlQt/ArrayBounds.h"
#include "TomlQt/TomlQt.h"

#include <optional>
#include <string_view>
#include <toml++/impl/array.hpp>
#include <toml++/toml.hpp>
#include <Qt>

class QSizePolicy;
class QSize;

// Use if return value and defaulting must be handled manually
// On success: extract from a node, return as std::optional<T>
// On failure: return std::nullopt
template<typename T>
std::optional<T> resolve(std::initializer_list<Source> sources, const PathContext& path_context,
                         const tomlqt::ArrayBounds& arr_bounds, const QString& arr_format) {
        using DT = std::decay_t<T>;

        // Convert pointers to std::optional
        // TODO Eliminate this lambda, toml::array and toml::table should be pointers
        //      But as long as the return type of resolve is stuck as std::optional,
        //      this cannot be done.
        static auto normalize = [&](auto&& raw) -> std::optional<T> {
                using R = std::decay_t<decltype(raw)>;
                if constexpr (std::is_same_v<R, std::optional<DT>>) {
                        return raw;
                } else if constexpr (std::is_pointer_v<R>) {
                        return raw ? std::optional<DT>(*raw) : std::nullopt;
                } else {
                        static_assert(false, "Unknown accessor return type!");
                }
        };

        // Collapse extraction logic into that of a corresponding type
        static auto extract = [&](node_view                  node,
                                  const tomlqt::ArrayBounds& arr_bounds = {}) -> std::optional<DT> {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        return normalize(node.as_table());
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        // TODO Extract as a separate function
                        using result = tomlqt::ArrayBounds::validation_result;

                        std::optional<toml::array> arr = normalize(node.as_array());
                        if (!arr) { return std::nullopt; }

                        auto res = arr_bounds.validate(arr.value());
                        if (res == result::min_size_fail) {
                                QWARNING()
                                        << QString("arr size < min_size! min_size: %1, arr size: %2")
                                                   .arg(QString::number(arr_bounds.min_size.value()),
                                                        QString::number(arr->size()));
                                return std::nullopt;
                        }

                        if (res == result::max_size_fail) {
                                QWARNING()
                                        << QString("arr size > max_size! max_size: %1, arr size: %2")
                                                   .arg(QString::number(arr_bounds.max_size.value()),
                                                        QString::number(arr->size()));
                                return std::nullopt;
                        }
                        return std::move(arr);
                } else if constexpr (std::is_same_v<DT, QSize>) {
                        return normalize(tomlqt::tryGetQSize(node));
                } else if constexpr (std::is_same_v<DT, Qt::Alignment>) {
                        return normalize(tomlqt::tryGetQtAlignment(node));
                } else if constexpr (std::is_same_v<DT, QSizePolicy>) {
                        return normalize(tomlqt::tryGetQSizePolicy(node));
                } else if constexpr (std::is_same_v<DT, QString>) {
                        return normalize(tomlqt::tryGetQString(node));
                } else {
                        return normalize(node.value<DT>());
                }
        };

        // Collapse logging message variants
        static auto log = [&](const QString& path) {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        QWARNING()
                                << QString("%1, missing or wrong type! Using defaults...").arg(path);
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        QWARNING()
                                << QString("%1, missing or wrong type! Format: %2. Using defaults...")
                                           .arg(path, QString("must be an array! Format: %1")
                                                              .arg(arr_format));
                } else {
                        QWARNING()
                                << QString("%1, missing or wrong type! Using defaults...").arg(path);
                }
        };

        // Validate and attempt extraction of each passed source, prioritizing earliest ones
        const auto* src_ptr = sources.begin();
        for (size_t i = 0; i != sources.size(); ++i) {
                const auto& source = src_ptr[i];

                // If 'i' is not the last index, then sources[i] is an override
                // TODO If primary source is valid and fallback is not then quiet on
                //      primary source will result in no logs being printed at all
                //      Iteration over sources should probably be done in reverse to
                //      track if fallback is missing, and if it is then quiet should
                //      likely be ignored.
                bool is_override  = (i != sources.size() - 1);
                // If override or explicitly marked "quiet", don't log anything
                bool silence_logs = is_override || source.quiet;

                auto result = extract(source.node, arr_bounds);
                if (!result) {
                        if (!silence_logs) { log(path_context.makePath(source.scope)); }
                        continue;
                }

                QDEBUG() << path_context.makePath(source.scope) << "found!";
                return *result;
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}

// Use to skip validation of return value and to automatically default
// On success: extract from a node
// On failure: copy default value
template<typename T, typename TDefault>
T resolveOr(std::initializer_list<Source> sources, const TDefault& defaults,
            const PathContext& path_context, const tomlqt::ArrayBounds& arr_bounds,
            const QString& arr_format) {
        return resolve<T>(sources, path_context, arr_bounds, arr_format).value_or(defaults);
}

// Use to try and extract a value from a node into a specific attribute, and if that fails, to
// default a completely different object
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: write result into a provided attribute
// On failure: overwrite object with object_defaults entirely
template<typename TAttribute, typename TObject>
void resolveOrDefault(std::initializer_list<Source> sources, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, const PathContext& path_context,
                      const tomlqt::ArrayBounds& arr_bounds, const QString& arr_format) {
        if (auto result = resolve<TAttribute>(sources, path_context, arr_bounds, arr_format)) {
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
                               Transform&& transform, const PathContext& path_context) {
        if (auto result = resolve<TRaw>(sources, path_context)) {
                attribute = transform(std::move(result.value()));
        } else {
                object = object_defaults;
        }
}
