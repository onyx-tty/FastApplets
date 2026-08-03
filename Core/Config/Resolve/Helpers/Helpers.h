// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/NodeView.h"

#include <TomlQt/ArrayBounds.h>
#include <concepts>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QStringView>

namespace config::resolve::detail {

using tomlqt::ArrayBounds;

template<typename T> concept ReturnPtr = std::is_same_v<T, toml::table>
                                      || std::is_same_v<T, toml::array>;

template<typename T>
requires(ReturnPtr<std::decay_t<T>>)
const std::decay_t<T>* extract(node_view node, const ArrayBounds& arr_bounds);

template<typename T>
requires(!ReturnPtr<std::decay_t<T>>)
std::optional<std::decay_t<T>> extract(node_view node);

template<typename T>
requires(std::is_same_v<toml::array, std::decay_t<T>>)
void log(QStringView path, QStringView arr_format);

template<typename T>
requires(!std::is_same_v<toml::array, std::decay_t<T>>)
void log(QStringView path);

} // namespace config::resolve::detail

#include "Helpers.tpp"
