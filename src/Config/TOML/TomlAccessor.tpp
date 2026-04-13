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

#include "Log/Log.h"
#include "CppUtils/include/Enum.h"
#include "CppUtils/include/String.h"
#include "TomlAccessor.h"
#include "Types/NodeView.h"

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

template<typename T>
T getValueFromEnumMap(const std::string& key, const EnumMap<T>& map, const T& fallback,
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
std::optional<T> tryGetValueFromEnumMap(const std::string& key, const enum_utils::EnumMap<T>& map,
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
