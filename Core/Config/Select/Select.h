// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Types/Candidates.h"
#include "Core/Config/Types/ArraySpec.h"
#include "Core/Config/Types/NodeView.h"

namespace config::select::helpers {}

namespace config::select {

using namespace config::select::helpers;
using config::ArraySpec;

// TODO: Docs
template<typename TReturn>
node_view candidate(const Candidates& candidates, const ArraySpec& array_spec = {});

} // namespace config::select

#include "Select.tpp"
