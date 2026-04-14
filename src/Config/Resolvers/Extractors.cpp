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

#include "Extractors.h"
#include "Config/TOML/Types/NodeView.h"
#include "Config/TOML/Types/TomlArrayConditions.h"

#include <optional>
#include <qnamespace.h>
#include <string>
#include <QSize>
#include <QSizePolicy>
#include <QString>

/* Extractors */
std::optional<toml::table> extractor::table(node_view node, const QString& path, bool is_override) {
        if (auto* result = TomlAccessor::tryGetTomlTable(node, path, is_override)) {
                return *result;
        }

        return std::nullopt;
};

std::optional<toml::array> extractor::array(node_view node, const QString& path, bool is_override,
                                            const TomlArrayConditions& arr_conditions) {
        return TomlAccessor::tryGetTomlArray(node, path, is_override, arr_conditions);
};

std::optional<QString> extractor::qstring(node_view node, const QString& path, bool is_override) {
        if (auto str = extractor::value<std::string>(node, path, is_override)) {
                return QString::fromStdString(str.value());
        }

        return std::nullopt;
};

std::optional<QSize> extractor::qsize(node_view node, const QString& path, bool is_override) {
        return TomlAccessor::tryGetQSize(node, path, is_override);
};

std::optional<Qt::Alignment> extractor::alignment(node_view node, const QString& path,
                                                  bool is_override) {
        auto raw = TomlAccessor::tryGet<std::string>(node, path, is_override);
        if (!raw) { return std::nullopt; }
        return TomlAccessor::tryGetAlignment(raw.value(), alignment_map, path);
};

std::optional<QSizePolicy> extractor::size_policy(node_view node, const QString& path,
                                                  bool is_override) {
        auto raw = TomlAccessor::tryGet<std::string>(node, path, is_override);
        if (!raw) { return std::nullopt; }
        return TomlAccessor::tryGetSizePolicy(raw.value(), size_policy_map, path);
};
