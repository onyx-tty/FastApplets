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

#include "TomlAccessor.h"
#include "CppUtils/include/Log/QtLog.h"
#include "Types/NodeView.h"
#include "Types/TomlArrayConditions.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

std::optional<QSize> TomlAccessor::tryGetQSize(node_view node) {
        using result                    = TomlArrayConditions::validation_result;
        constexpr size_t min_size       = 2;
        const auto       arr_conditions = TomlArrayConditions{min_size};
        const auto*      arr            = node.as_array();

        if (!arr) { return std::nullopt; }
        auto res = arr_conditions.validate(*arr);
        if (res == result::min_size_fail) {
                QWARNING() << QString("QSize requires 2 numbers for construction! Numbers provided: %1")
                                      .arg(QString::number(arr->size()));
                return std::nullopt;
        }

        auto width  = (*arr)[0].value<int64_t>();
        auto height = (*arr)[1].value<int64_t>();

        if (!width || !height) { return std::nullopt; }

        return QSize(width.value(), height.value());
}

std::optional<QString> TomlAccessor::tryGetQString(node_view node) {
        if (auto str = node.value<std::string>()) { return QString::fromStdString(str.value()); }

        return std::nullopt;
}

std::optional<Qt::Alignment> TomlAccessor::tryGetAlignment(node_view node) {
        static const std::unordered_map<std::string, Qt::Alignment> alignment_map =
                {{"top", Qt::AlignTop | Qt::AlignHCenter},
                 {"center", Qt::AlignCenter},
                 {"bottom", Qt::AlignBottom | Qt::AlignHCenter},
                 {"left", Qt::AlignVCenter | Qt::AlignLeft},
                 {"right", Qt::AlignVCenter | Qt::AlignRight}};

        return TomlAccessor::tryGetValueFromEnumMap<Qt::Alignment>(node, alignment_map);
}

std::optional<QSizePolicy> TomlAccessor::tryGetSizePolicy(node_view node) {
        static const std::unordered_map<std::string, QSizePolicy> size_policy_map =
                {{"expanding", {QSizePolicy::Expanding, QSizePolicy::Expanding}},
                 {"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}}};

        return TomlAccessor::tryGetValueFromEnumMap<QSizePolicy>(node, size_policy_map);
}
