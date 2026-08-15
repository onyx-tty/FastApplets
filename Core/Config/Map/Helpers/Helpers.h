// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Candidates/Candidates.h"

#include <optional>
#include <toml++/toml.hpp>

namespace config::map::helpers {

using config::Candidates;

template<typename T>
[[nodiscard]] std::optional<T> table(const Candidates& candidates, auto fill_fn);

} // namespace config::map::helpers

#include "Helpers.tpp"
