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

#include "CppUtils/Log/QtLog.h"
#include "CppUtils/String/StdString.h"
#include "TomlAccessor.h"
#include "Types/NodeView.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QSize>
#include <QString>

template<typename T>
std::optional<T> TomlAccessor::tryGetValueFromEnumMap(
        node_view key, const std::unordered_map<std::string, T>& map) {
        using string::toLowerCopy;

        const auto key_str = key.value<std::string>();
        if (!key_str) { return std::nullopt; }

        const auto iter = map.find(toLowerCopy(key_str.value()));
        if (iter == map.cend()) {
                QWARNING() << QString("Key %1 not found in map!")
                                      .arg(QString::fromStdString(key_str.value()));
                return std::nullopt;
        }

        return iter->second;
}
