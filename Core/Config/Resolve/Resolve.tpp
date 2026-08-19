// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Resolve.h"
#include "Core/Config/Log/Log.h"
#include "Core/Config/Select/Select.h"
#include "Core/Config/Types/ArraySpec.h"
#include "Core/Config/Types/Candidates/Candidates.h"

#include <QStringView>
#include <TomlQt/ArrayBounds.h>
#include <TomlQt/TomlQt.h>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>

template<typename T>
requires(config::resolve::ReturnByValue<T>)
std::optional<T> config::resolve::from(const Candidates& candidates) {
        using DT = std::decay_t<T>;

        const auto* candidate = config::select::candidate<DT>(candidates);

        const bool is_success = candidate;
        log::candidate(candidate, is_success);

        if (!candidate) { return std::nullopt; }
        return tomlqt::value<DT>(candidate->node);
}

template<typename T>
requires(config::resolve::ReturnByView<T>)
const T* config::resolve::from(const Candidates& candidates, const ArraySpec& array_spec) {
        using DT                = std::decay_t<T>;
        using validation_result = ArrayBounds::validation_result;

        const auto*  candidate      = config::select::candidate<DT>(candidates);
        ArrayLogSpec array_log_spec = {.result = validation_result::null_ptr,
                .format                        = array_spec.format};

        // Arrays must pass the bound check first.
        if constexpr (std::same_as<toml::array, DT>) {
                if (candidate) {
                        const auto* as_arr = candidate ? candidate->node.as_array() : nullptr;

                        // Empty arr_bounds means there is no need for
                        // bound checking, therefore it's success by default.
                        array_log_spec.result = array_spec.bounds
                                                      ? array_spec.bounds.value().validate(as_arr)
                                                      : validation_result::success;
                }
        }

        const bool is_success = candidate;
        log::candidate(candidate, is_success, array_log_spec);

        return candidate ? candidate->node.template as<DT>() : nullptr;
}
