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

#include "TomlAccessor.h"
#include "Log/Log.h"
#include "Types/NodeView.h"
#include "Types/TomlArrayConditions.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <toml++/toml.hpp>
#include <QSize>
#include <QString>

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
                                        const TomlArrayConditions& arr_conditions) {
        const auto* arr = node.as_array();

        if (!arr) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", must be an array! "
                                      << arr_conditions.array_format << " Using defaults...";
                }

                return std::nullopt;
        }

        if (arr_conditions.min_size && arr->size() < arr_conditions.min_size.value()) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", arr size < min_size! min_size: "
                                      << arr_conditions.min_size.value()
                                      << ", size: " << arr->size() << ". Using defaults...";
                }

                return std::nullopt;
        }

        if (arr_conditions.max_size && arr->size() > arr_conditions.max_size.value()) {
                if (is_override) {
                        QDEBUG() << path
                                 << "is an override, and missing! Proceeding with globals...";
                } else {
                        QWARNING_NS() << path << ", arr size >= max_size! max_size: "
                                      << arr_conditions.max_size.value()
                                      << ", size: " << arr->size() << ". Using defaults...";
                }

                return std::nullopt;
        }

        return *arr;
}

QSize getQSize(node_view node, const QSize& fallback, const QString& path, bool is_override) {
        constexpr size_t min_size = 2;
        const auto       arr      = getTomlArray(node, path, is_override,
                                                 {"Format: [int, int]", min_size, std::nullopt});

        if (!arr) { return fallback; }

        int width  = getOrDefault<int64_t>(toml::node_view(arr.value()[0]), fallback.width(),
                                           path + "[0]");
        int height = getOrDefault<int64_t>(toml::node_view(arr.value()[1]), fallback.height(),
                                           path + "[1]");

        return QSize(width, height);
}

std::optional<QSize> tryGetQSize(node_view node, const QString& path, bool is_override) {
        constexpr size_t min_size = 2;
        const auto& arr = getTomlArray(node, path, is_override, {"Format: [int, int]", min_size});

        if (!arr) { return std::nullopt; }

        auto width  = tryGet<int64_t>(toml::node_view(arr.value()[0]), path + "[0]");
        auto height = tryGet<int64_t>(toml::node_view(arr.value()[1]), path + "[1]");

        if (!width || !height) { return std::nullopt; }

        return QSize(width.value(), height.value());
}
