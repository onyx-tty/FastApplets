// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Candidates/Candidates.h"

#include <optional>

namespace config::map::helpers {

// TODO: Document
template<typename TContainer, typename TValue, typename TExtension>
void field(TContainer& object, TValue TContainer::* member, const Candidates& candidates,
           const TContainer& defaults, TExtension extension, std::optional<bool> quiet = false);

} // namespace config::map::helpers

#include "Field.tpp"
