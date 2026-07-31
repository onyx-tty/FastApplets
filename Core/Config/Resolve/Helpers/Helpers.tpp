// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/NodeView.h"
#include "Helpers.h"

#include <TomlQt/ArrayBounds.h>
#include <TomlQt/TomlQt.h>
#include <optional>
#include <type_traits>

template<typename T>
requires(config::resolve::detail::ReturnPtr<std::decay_t<T>>)
const std::decay_t<T>* config::resolve::detail::extract(node_view          node,
                                                        const ArrayBounds& arr_bounds) {
        using DT = std::decay_t<T>;

        if constexpr (std::is_same_v<DT, toml::table>) {
                return node.as_table();
        } else if constexpr (std::is_same_v<DT, toml::array>) {
                return tomlqt::asArrayWithBounds(node, arr_bounds);
        }
};

template<typename T>
requires(!config::resolve::detail::ReturnPtr<std::decay_t<T>>)
std::optional<std::decay_t<T>> config::resolve::detail::extract(node_view node) {
        using DT = std::decay_t<T>;

        return tomlqt::value<DT>(node);
};
