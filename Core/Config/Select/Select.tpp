// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Select.h"

#include "Core/Config/Log/Log.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"

#include <TomlQt/ArrayBounds.h>
#include <TomlQt/TomlQt.h>
#include <concepts>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QDebug>
#include <QtGlobal>

template<typename TReturn>
requires(tomlqt::TomlQtSupported<std::decay_t<TReturn>>)
node_view config::select::candidate(const Candidates& candidates, const ArraySpec& array_spec) {
        using DTReturn          = std::decay_t<TReturn>;
        using validation_result = tomlqt::ArrayBounds::validation_result;
        using config::log::ArrayLogSpec;

        for (const auto& candidate : candidates.get()) {
                bool is_success    = false;
                bool is_node_valid = false;
                if constexpr (tomlqt::TomlSupported<DTReturn>) {
                        is_node_valid = candidate.node.as<DTReturn>();
                } else { // TODO: Create tomlqt::as instead, repeated tomlqt::value is too heavy
                        is_node_valid = tomlqt::value<DTReturn>(node_view(candidate.node))
                                                .has_value();
                }

                ArrayLogSpec array_log_spec = {.result = validation_result::null_ptr,
                                               .format = array_spec.format};

                if (is_node_valid) {
                        if constexpr (std::same_as<toml::array, DTReturn>) {
                                // Empty arr_bounds means there is no need for
                                // bound checking therefore it's success by default.
                                array_log_spec.result = array_spec.bounds
                                                              ? array_spec.bounds.value().validate(
                                                                        candidate.node.as_array())
                                                              : validation_result::success;

                                is_success = (array_log_spec.result == validation_result::success);
                        } else {
                                is_success = true;
                        }
                }

                if constexpr (std::same_as<toml::array, DTReturn>) {
                        log::candidate(candidate, is_success, array_log_spec);
                } else {
                        log::candidate(candidate, is_success);
                }

                if (is_node_valid) { return candidate.node; }
        }

        return {};
}
