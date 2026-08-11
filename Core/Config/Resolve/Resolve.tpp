// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Select.h"
#include "Core/Config/Select/Types/Candidates.h"
#include "Resolve.h"

#include <TomlQt/TomlQt.h>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QStringView>

template<typename T>
requires(!std::is_same_v<T, toml::table> && !std::is_same_v<T, toml::array>)
std::optional<T> config::resolve::from(const Candidates& candidates) {
        using DT = std::decay_t<T>;

        auto result = config::select::candidate<DT>(candidates);

        return tomlqt::value<DT>(result);
}

template<typename T>
requires(std::is_same_v<std::decay_t<T>, toml::table>
         || std::is_same_v<std::decay_t<T>, toml::array>)
const T* config::resolve::from(const Candidates& candidates, const ArrayBounds& arr_bounds,
                               QStringView arr_format) {
        using DT = std::decay_t<T>;

        auto result = config::select::candidate<DT>(candidates, arr_bounds, arr_format);

        return result.template as<DT>();
}
