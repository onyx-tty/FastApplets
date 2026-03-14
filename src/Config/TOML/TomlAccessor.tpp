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
T getOrDefault(node_view node, const T& fallback, const QString& error_prefix) {
        const auto* value = node.as<T>();

        if (!value) {
                QWARNING_NS() << error_prefix << ", missing or wrong type! Using defaults...";
                return fallback;
        }

        return value->get();
}

const toml::table* getTable(node_view node, const QString& error_prefix) {
        const auto* table = node.as_table();
        if (!table) { QWARNING_NS() << error_prefix << ", must be a table! Using defaults..."; }

        return table;
}

std::optional<toml::array> getTomlArray(node_view node, const QString& error_prefix,
                                        const QString& error_arr_details) {
        const auto* arr = node.as_array();

        if (!arr) {
                QWARNING_NS() << error_prefix << ", must be an array! " << error_arr_details
                              << " Using defaults...";
                return std::nullopt;
        }

        return *arr;
}

std::optional<toml::array> getTomlArray(node_view node, size_t min_size,
                                        const QString& error_prefix,
                                        const QString& error_arr_details) {
        const auto* arr = node.as_array();

        if (!arr || arr->size() < min_size) {
                QWARNING_NS() << error_prefix << ", must be an array! " << error_arr_details
                              << " Using defaults...";
                return std::nullopt;
        }

        return *arr;
}

std::optional<toml::array> getTomlArray(node_view node, size_t min_size, size_t max_size,
                                        const QString& error_prefix,
                                        const QString& error_arr_details) {
        const auto* arr = node.as_array();

        if (!arr || arr->size() < min_size || arr->size() > max_size) {
                QWARNING_NS() << error_prefix << ", must be an array! " << error_arr_details
                              << " Using defaults...";
                return std::nullopt;
        }

        return *arr;
}

QSize getQSize(node_view node, const QSize& fallback, const QString& error_prefix) {
        const auto& arr = getTomlArray(node, 2, error_prefix, "Format: [int, int]");

        if (!arr) { return fallback; }

        int width  = getOrDefault<int64_t>(toml::node_view(arr.value()[0]), fallback.width(),
                                           error_prefix + "[0]");
        int height = getOrDefault<int64_t>(toml::node_view(arr.value()[1]), fallback.height(),
                                           error_prefix + "[1]");

        return QSize(width, height);
}

template<typename T>
T getValueFromEnumMap(const std::string key, const EnumMap<T>& map, const T& fallback,
                      const QString& error_prefix) {
        if (!map.contains(key)) {
                QWARNING() << error_prefix << "invalid! Using defaults...";
                return fallback;
        }

        return map.at(toLowerCopy(key));
}
