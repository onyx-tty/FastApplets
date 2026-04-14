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

#include <optional>
#include <toml++/toml.hpp>
#include <QSizePolicy>

class QString;

namespace extractor {
const toml::table* table(node_view node, const QString& path, bool is_override = false);

const toml::array* array(node_view node, const QString& path, bool is_override = false,
                         const TomlArrayConditions& arr_conditions = {});

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
