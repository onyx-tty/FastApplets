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

#include "CppUtils/include/Enum/Enum.h"
#include "Types/NodeView.h"
#include "Types/TomlArrayConditions.h"

#include <optional>
#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

class TomlAccessor final {
public:
        TomlAccessor() = delete;
        static const toml::array*   tryGetTomlArray(node_view                  node,
                                                    const TomlArrayConditions& arr_conditions = {});
        static std::optional<QSize> tryGetQSize(node_view node);
        static std::optional<QString> tryGetQString(node_view node);
        template<typename T>
        static std::optional<T> tryGetValueFromEnumMap(node_view key, const enums::EnumMap<T>& map);
        static std::optional<Qt::Alignment> tryGetAlignment(node_view node);
        static std::optional<QSizePolicy>   tryGetSizePolicy(node_view node);
};

#include "TomlAccessor.tpp"
