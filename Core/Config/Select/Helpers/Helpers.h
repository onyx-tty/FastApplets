// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Types/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QStringView>

namespace config::select::helpers {

using validation_result = tomlqt::ArrayBounds::validation_result;

template<typename T>
requires(std::is_same_v<toml::array, std::decay_t<T>>)
void log(const Candidate& candidate, validation_result arr_result, QStringView arr_format);

template<typename T>
requires(!std::is_same_v<toml::array, std::decay_t<T>>)
void log(const Candidate& candidate);

} // namespace config::select::helpers

#include "Helpers.tpp"
