// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/ArraySpec.h"
#include "Core/Config/Types/Candidates/Candidate.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"

#include <TomlQt/TomlQt.h>
#include <type_traits>

namespace config::select {

using config::ArraySpec;
using namespace config;

// TODO: Docs
template<typename TReturn>
requires(tomlqt::TomlQtSupported<std::decay_t<TReturn>>)
const Candidate* candidate(const Candidates& candidates, const ArraySpec& array_spec = {});

} // namespace config::select

#include "Select.tpp"
