// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/ErrorType.h"
#include "Core/Config/Types/NodeView.h"

#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>

namespace tomlqt { struct ArrayBounds; } // namespace tomlqt

namespace config::validate {

using config::ErrorType;
using tomlqt::ArrayBounds;

// Returns std::optional<ErrorType>, where invalid node results in ErrorType::Missing,
// type unsupported by TomlQt in ErrorType::InvalidType, and success in std::nullopt.
template<typename TExpected>
std::optional<ErrorType> node(node_view view);

// Returns std::optional<ErrorType>, where invalid node results in ErrorType::Missing,
// type unsupported by TomlQt in InvalidType, array_bounds failing the bound check in
// ArrayOutOfBounds, and success in std::nullopt.
template<typename TExpected>
requires(std::is_same_v<TExpected, toml::array>)
std::optional<ErrorType> node(node_view view, const ArrayBounds& array_bounds);

} // namespace config::validate

#include "Validate.tpp"
