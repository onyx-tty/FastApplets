// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Helpers.h"
#include "Core/Config/Log/Log.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Types/Candidates/Candidates.h"

#include <optional>
#include <toml++/toml.hpp>

template<typename T>
std::optional<T> config::map::helpers::table(const Candidates& candidates, auto fill_fn) {
        using namespace config;

        const toml::table* properties = nullptr;

        for (const auto& candidate : candidates.get()) {
                properties = candidate.node.as_table();

                config::log::candidate(&candidate, properties);

                if (properties) { break; }
        }

        auto props = T();
        fill_fn(props);

        return std::move(props);
}

template<typename TContainer, typename TValue, typename TExtension>
void config::map::helpers::field(TContainer& object, TValue TContainer::* member,
        const Candidates& candidates, const TContainer& defaults, TExtension extension) {
        auto cands     = candidates[extension];
        object.*member = resolve::from<TValue>(cands).value_or(defaults.*member);
}
