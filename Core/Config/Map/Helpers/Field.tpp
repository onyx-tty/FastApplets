// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Field.h"

#include <optional>

template<typename TContainer, typename TValue, typename TExtension>
void config::map::helpers::field(TContainer& object, TValue TContainer::* member,
                                 const Candidates& candidates, const TContainer& defaults,
                                 TExtension extension, std::optional<bool> quiet) {
        auto cands = candidates.makeCopy().withExtension(extension);
        if (quiet) { cands.withQuiet(quiet.value()); }

        object.*member = resolve::from<TValue>(cands).value_or(defaults.*member);
}
