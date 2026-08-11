// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Types/Candidates.h"
#include "Core/Config/Types/NodeView.h"

#include <TomlQt/ArrayBounds.h>
#include <optional>
#include <toml++/toml.hpp>
#include <QStringView>

namespace config::select::helpers {}

namespace config::select {

using namespace config::select::helpers;
using tomlqt::ArrayBounds;

// TODO: Docs
template<typename TReturn>
node_view candidate(const Candidates& candidates, std::optional<ArrayBounds> arr_bounds = {},
                    QStringView arr_format = {});

} // namespace config::select

#include "Select.tpp"
