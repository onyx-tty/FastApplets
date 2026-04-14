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

#include "CppUtils/include/Enum.h"
#include "Types/NodeView.h"
#include "Types/TomlArrayConditions.h"

#include <optional>
#include <qnamespace.h>
#include <string>
#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>

extern const enum_utils::EnumMap<Qt::Alignment> alignment_map;
extern const enum_utils::EnumMap<QSizePolicy>   size_policy_map;

class TomlAccessor final {
public:
        TomlAccessor() = delete;
        template<typename T>
        static T getOrDefault(node_view node, const T& fallback, const QString& path,
                              bool is_override = false);
        template<typename T>
        static std::optional<T>           tryGet(node_view node, const QString& path,
                                                 bool is_override = false);
        static const toml::table*         tryGetTomlTable(node_view node, const QString& path,
                                                          bool is_override = false);
        static std::optional<toml::array> tryGetTomlArray(
                node_view node, const QString& path, bool is_override = false,
                const TomlArrayConditions& arr_conditions = {});
        static QSize getQSize(node_view node, const QSize& fallback, const QString& path,
                              bool is_override = false);
        static std::optional<QSize> tryGetQSize(node_view node, const QString& path,
                                                bool is_override = false);
        template<typename T>
        static T getValueFromEnumMap(const std::string& key, const enum_utils::EnumMap<T>& map,
                                     const T& fallback, const QString& path,
                                     bool is_override = false);
        template<typename T>
        static std::optional<T> tryGetValueFromEnumMap(const std::string&            key,
                                                       const enum_utils::EnumMap<T>& map,
                                                       const QString&                path,
                                                       bool is_override = false);
        static Qt::Alignment    getAlignment(const std::string&                        key,
                                             const enum_utils::EnumMap<Qt::Alignment>& map,
                                             const Qt::Alignment& fallback, const QString& path);
        static std::optional<Qt::Alignment> tryGetAlignment(
                const std::string& key, const enum_utils::EnumMap<Qt::Alignment>& map,
                const QString& path);
        static QSizePolicy getSizePolicy(const std::string&                      key,
                                         const enum_utils::EnumMap<QSizePolicy>& map,
                                         const QSizePolicy& fallback, const QString& path);
        static std::optional<QSizePolicy> tryGetSizePolicy(
                const std::string& key, const enum_utils::EnumMap<QSizePolicy>& map,
                const QString& path);
};

#include "TomlAccessor.tpp"
