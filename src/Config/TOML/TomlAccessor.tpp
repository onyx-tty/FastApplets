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

void logMissing(const QString& path, bool is_override, const QString& detail) {
        if (is_override) {
                QDEBUG() << path << "is an override, and missing! Proceeding with globals...";
        } else {
                QWARNING() << QString("%1,%2 missing or wrong type! Using defaults...").arg(path, detail);
        }
}

template<typename T>
std::optional<T> TomlAccessor::tryGet(node_view node, const QString& path, bool is_override) {
        const auto* value = node.as<T>();

        if (!value) {
                logMissing(path, is_override);

                return std::nullopt;
        }

        QDEBUG() << path << "found!";
        return value->get();
}

template<typename T>
std::optional<T> TomlAccessor::tryGetValueFromEnumMap(node_view key, const enums::EnumMap<T>& map,
                                                      const QString& path, bool is_override) {
        using string::toLowerCopy;

        const auto key_str = tryGet<std::string>(key, path, is_override);
        if (!key_str) { return std::nullopt; }

        if (!map.contains(toLowerCopy(key_str.value()))) {
                logMissing(path, is_override);

                return std::nullopt;
        }

        return map.at(toLowerCopy(key_str.value()));
}
