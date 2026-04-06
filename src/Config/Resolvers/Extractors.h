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

#include "Config/TOML/NodeView.h"
#include "Config/TOML/TomlAccessor.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <QSizePolicy>

class QString;

extern const EnumMap<Qt::Alignment> alignment_map;
extern const EnumMap<QSizePolicy>   size_policy_map;

Qt::Alignment getAlignment(const std::string& key, const EnumMap<Qt::Alignment>& map,
                           const Qt::Alignment& fallback, const QString& path);

std::optional<Qt::Alignment> tryGetAlignment(const std::string&            key,
                                             const EnumMap<Qt::Alignment>& map,
                                             const QString&                path);

QSizePolicy getSizePolicy(const std::string& key, const EnumMap<QSizePolicy>& map,
                          const QSizePolicy& fallback, const QString& path);

std::optional<QSizePolicy> tryGetSizePolicy(const std::string& key, const EnumMap<QSizePolicy>& map,
                                            const QString& path);

namespace extractor {
std::optional<toml::table> table(node_view node, const QString& path, bool is_override = false);

std::optional<toml::array> array(node_view node, const QString& path, bool is_override = false,
                                 const QString& error_arr_details = {});

template<typename T>
std::optional<T> value(node_view node, const QString& path, bool is_override);

std::optional<QString> qstring(node_view node, const QString& path, bool is_override = false);

std::optional<QSize> qsize(node_view node, const QString& path, bool is_override = false);

std::optional<Qt::Alignment> alignment(node_view node, const QString& path,
                                       bool is_override = false);

std::optional<QSizePolicy> size_policy(node_view node, const QString& path,
                                       bool is_override = false);
} // namespace extractor

#include "Extractors.tpp"
