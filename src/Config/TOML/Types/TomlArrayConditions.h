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

#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>
#include <QString>

struct TomlArrayConditions final {
        std::optional<size_t> min_size     = std::nullopt;
        std::optional<size_t> max_size     = std::nullopt;
        enum class validation_result { success, min_size_fail, max_size_fail };

        validation_result validate(const toml::array& arr) const {
                if (min_size && min_size > arr.size()) { return validation_result::min_size_fail; }
                if (max_size && max_size < arr.size()) { return validation_result::max_size_fail; }
                return validation_result::success;
        }
};
