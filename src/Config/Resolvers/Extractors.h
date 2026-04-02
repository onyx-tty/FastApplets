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

Qt::Alignment getAlignment(const std::string key, const EnumMap<Qt::Alignment>& map,
                           const Qt::Alignment& fallback, const QString& path);

std::optional<Qt::Alignment> tryGetAlignment(const std::string             key,
                                             const EnumMap<Qt::Alignment>& map,
                                             const QString&                path);

QSizePolicy getSizePolicy(const std::string key, const EnumMap<QSizePolicy>& map,
                          const QSizePolicy& fallback, const QString& path);

std::optional<QSizePolicy> tryGetSizePolicy(const std::string key, const EnumMap<QSizePolicy>& map,
                                            const QString& path);

namespace extractor {
auto table = [](node_view node, const QString& path) -> std::optional<toml::table> {
        if (auto* result = getTomlTable(node, path)) { return *result; }

        return std::nullopt;
};

auto array = [](node_view node, const QString& path, const QString& error_arr_details = {}) {
        return getTomlArray(node, path, error_arr_details);
};

template<typename T>
auto value = [](node_view node, const QString& path) { return tryGet<T>(node, path); };

auto qstring = [](node_view node, const QString& path) -> std::optional<QString> {
        if (auto str = extractor::value<std::string>(node, path)) {
                return QString::fromStdString(str.value());
        }

        return std::nullopt;
};

auto qsize = [](node_view node, const QString& path) { return tryGetQSize(node, path); };

auto alignment = [](node_view node, const QString& path) -> std::optional<Qt::Alignment> {
        auto raw = tryGet<std::string>(node, path);
        if (!raw) { return std::nullopt; }
        return tryGetAlignment(raw.value(), alignment_map, path);
};

auto size_policy = [](node_view node, const QString& path) -> std::optional<QSizePolicy> {
        auto raw = tryGet<std::string>(node, path);
        if (!raw) { return std::nullopt; }
        return tryGetSizePolicy(raw.value(), size_policy_map, path);
};
} // namespace extractor
