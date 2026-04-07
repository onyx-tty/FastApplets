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
#include "CppUtils/include/Enum.h"

#include <qnamespace.h>
#include <string>
#include <QSizePolicy>
#include <QString>

const EnumMap<Qt::Alignment> alignment_map = {{"top", Qt::AlignTop | Qt::AlignHCenter},
                                              {"center", Qt::AlignCenter},
                                              {"bottom", Qt::AlignBottom | Qt::AlignHCenter},
                                              {"left", Qt::AlignVCenter | Qt::AlignLeft},
                                              {"right", Qt::AlignVCenter | Qt::AlignRight}};

const EnumMap<QSizePolicy> size_policy_map = {{"expanding",
                                               {QSizePolicy::Expanding, QSizePolicy::Expanding}},
                                              {"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}}};

Qt::Alignment getAlignment(const std::string& key, const EnumMap<Qt::Alignment>& map,
                           const Qt::Alignment& fallback, const QString& path) {
        return getValueFromEnumMap<Qt::Alignment>(key, map, fallback, path);
}

std::optional<Qt::Alignment> tryGetAlignment(const std::string&            key,
                                             const EnumMap<Qt::Alignment>& map,
                                             const QString&                path) {
        return tryGetValueFromEnumMap<Qt::Alignment>(key, map, path);
}

QSizePolicy getSizePolicy(const std::string& key, const EnumMap<QSizePolicy>& map,
                          const QSizePolicy& fallback, const QString& path) {
        return getValueFromEnumMap<QSizePolicy>(key, map, fallback, path);
}

std::optional<QSizePolicy> tryGetSizePolicy(const std::string& key, const EnumMap<QSizePolicy>& map,
                                            const QString& path) {
        return tryGetValueFromEnumMap<QSizePolicy>(key, map, path);
}

/* Extractors */
std::optional<toml::table> extractor::table(node_view node, const QString& path, bool is_override) {
        if (auto* result = getTomlTable(node, path, is_override)) { return *result; }

        return std::nullopt;
};

std::optional<toml::array> extractor::array(node_view node, const QString& path, bool is_override,
                                            const QString& error_arr_details) {
        return getTomlArray(node, path, is_override, error_arr_details);
};

std::optional<QString> extractor::qstring(node_view node, const QString& path, bool is_override) {
        if (auto str = extractor::value<std::string>(node, path, is_override)) {
                return QString::fromStdString(str.value());
        }

        return std::nullopt;
};

std::optional<QSize> extractor::qsize(node_view node, const QString& path, bool is_override) {
        return tryGetQSize(node, path, is_override);
};

std::optional<Qt::Alignment> extractor::alignment(node_view node, const QString& path,
                                                  bool is_override) {
        auto raw = tryGet<std::string>(node, path, is_override);
        if (!raw) { return std::nullopt; }
        return tryGetAlignment(raw.value(), alignment_map, path);
};

std::optional<QSizePolicy> extractor::size_policy(node_view node, const QString& path,
                                                  bool is_override) {
        auto raw = tryGet<std::string>(node, path, is_override);
        if (!raw) { return std::nullopt; }
        return tryGetSizePolicy(raw.value(), size_policy_map, path);
};
