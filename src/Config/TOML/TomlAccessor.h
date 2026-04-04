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
#include "NodeView.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <QSize>
#include <QString>

template<typename T>
T getOrDefault(node_view node, const T& fallback, const QString& path, bool is_override = false);

template<typename T>
std::optional<T> tryGet(node_view node, const QString& path, bool is_override = false);

const toml::table* getTomlTable(node_view node, const QString& path, bool is_override = false);

std::optional<toml::array> getTomlArray(node_view node, const QString& path,
                                        bool                  is_override       = false,
                                        const QString&        error_arr_details = {},
                                        std::optional<size_t> min_size          = std::nullopt,
                                        std::optional<size_t> max_size          = std::nullopt);

QSize getQSize(node_view node, const QSize& fallback, const QString& path, bool is_override = false);

std::optional<QSize> tryGetQSize(node_view node, const QString& path, bool is_override = false);

template<typename T>
T getValueFromEnumMap(const std::string key, const enum_utils::EnumMap<T>& map, const T& fallback,
                      const QString& path, bool is_override = false);

template<typename T>
std::optional<T> tryGetValueFromEnumMap(const std::string key, const enum_utils::EnumMap<T>& map,
                                        const QString& path, bool is_override = false);

#include "TomlAccessor.tpp"
