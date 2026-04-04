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

#include "Core/Log.h"
#include "CppUtils/include/Enum.h"
#include "CppUtils/include/String.h"
#include "TomlAccessor.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <QSize>
#include <QString>

using enum_utils::EnumMap;
using string_utils::toLowerCopy;

template<typename T>
T getOrDefault(node_view node, const T& fallback, const QString& path, bool is_override) {
        const auto* value = node.as<T>();

        if (!value) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", missing or wrong type! Using defaults...";
                }

                return fallback;
        }

        return value->get();
}

template<typename T>
std::optional<T> tryGet(node_view node, const QString& path, bool is_override) {
        const auto* value = node.as<T>();

        if (!value) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", missing or wrong type! Using defaults...";
                }

                return std::nullopt;
        }

        return value->get();
}

const toml::table* getTomlTable(node_view node, const QString& path, bool is_override) {
        const auto* table = node.as_table();
        if (!table) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", must be a table! Using defaults...";
                }

                return nullptr;
        }

        return table;
}

std::optional<toml::array> getTomlArray(node_view node, const QString& path, bool is_override,
                                        const QString&        error_arr_details,
                                        std::optional<size_t> min_size,
                                        std::optional<size_t> max_size) {
        const auto* arr = node.as_array();

        if (!arr) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", must be an array! " << error_arr_details
                                      << " Using defaults...";
                }

                return std::nullopt;
        }

        if (min_size && arr->size() < min_size.value()) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS()
                                << path << ", arr size < min_size! min_size: " << min_size.value()
                                << ", size: " << arr->size() << ". Using defaults...";
                }

                return std::nullopt;
        }

        if (max_size && arr->size() > max_size.value()) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS()
                                << path << ", arr size >= max_size! max_size: " << max_size.value()
                                << ", size: " << arr->size() << ". Using defaults...";
                }

                return std::nullopt;
        }

        return *arr;
}

QSize getQSize(node_view node, const QSize& fallback, const QString& path, bool is_override) {
        constexpr size_t min_size = 2;
        const auto       arr = getTomlArray(node, path, is_override, "Format: [int, int]", min_size,
                                            std::nullopt);

        if (!arr) { return fallback; }

        int width  = getOrDefault<int64_t>(toml::node_view(arr.value()[0]), fallback.width(),
                                           path + "[0]");
        int height = getOrDefault<int64_t>(toml::node_view(arr.value()[1]), fallback.height(),
                                           path + "[1]");

        return QSize(width, height);
}

std::optional<QSize> tryGetQSize(node_view node, const QString& path, bool is_override) {
        constexpr size_t min_size = 2;
        const auto& arr = getTomlArray(node, path, is_override, "Format: [int, int]", min_size);

        if (!arr) { return std::nullopt; }

        auto width  = tryGet<int64_t>(toml::node_view(arr.value()[0]), path + "[0]");
        auto height = tryGet<int64_t>(toml::node_view(arr.value()[1]), path + "[1]");

        if (!width || !height) { return std::nullopt; }

        return QSize(width.value(), height.value());
}

template<typename T>
T getValueFromEnumMap(const std::string key, const EnumMap<T>& map, const T& fallback,
                      const QString& path, bool is_override) {
        if (!map.contains(key)) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", invalid! Using defaults...";
                }

                return fallback;
        }

        return map.at(toLowerCopy(key));
}

template<typename T>
std::optional<T> tryGetValueFromEnumMap(const std::string key, const enum_utils::EnumMap<T>& map,
                                        const QString& path, bool is_override) {
        if (!map.contains(key)) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", invalid! Using defaults...";
                }

                return std::nullopt;
        }

        return map.at(toLowerCopy(key));
}
