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
#include "CppUtils/include/String/String.h"
#include "Log/Log.h"
#include "TomlAccessor.h"
#include "Types/NodeView.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <QSize>
#include <QString>

template<typename T>
std::optional<T> TomlAccessor::tryGet(node_view node) {
        const auto* value = node.as<T>();

        if (!value) { return std::nullopt; }

        return value->get();
}

template<typename T>
std::optional<T> TomlAccessor::tryGetValueFromEnumMap(node_view key, const enums::EnumMap<T>& map) {
        using string::toLowerCopy;

        const auto key_str = tryGet<std::string>(key);
        if (!key_str) { return std::nullopt; }

        if (!map.contains(toLowerCopy(key_str.value()))) {
                QWARNING() << QString("Key %1 not found in map!")
                                      .arg(QString::fromStdString(key_str.value()));
                return std::nullopt;
        }

        return map.at(toLowerCopy(key_str.value()));
}
