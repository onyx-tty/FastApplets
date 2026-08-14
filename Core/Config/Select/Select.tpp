// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Select.h"

#include "Core/Config/Log/Log.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"

#include <TomlQt/ArrayBounds.h>
#include <cstddef>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QDebug>
#include <QtGlobal>

template<typename TReturn>
node_view config::select::candidate(const Candidates& candidates, const ArraySpec& array_spec) {
        using DTReturn          = std::decay_t<TReturn>;
        using validation_result = tomlqt::ArrayBounds::validation_result;

        const auto candidate_ptr = candidates.get().begin();
        for (size_t i = 0; i != candidates.get().size(); ++i) {
                const auto& candidate = candidate_ptr[i];

                // If 'i' is not the last index, then candidates[i] is an override
                // TODO If primary candidate is valid and fallback is not then quiet on
                //      primary candidate will result in no logs being printed at all
                //      Iteration over candidates should probably be done in reverse to
                //      track if fallback is missing, and if it is then quiet should
                //      likely be ignored.
                bool is_override = (i != candidates.get().size() - 1);

                // If override or explicitly marked "quiet", don't log anything
                bool silence_logs = is_override || candidate.quiet;

                if (!candidate.node) {
                        if (!silence_logs) {
                                if constexpr (std::is_same_v<toml::array, DTReturn>) {
                                        // Empty arr_bounds means there is no need for
                                        // bound checking therefore it's success by default.
                                        auto res = array_spec.bounds
                                                         ? array_spec.bounds.value().validate(
                                                                   candidate.node.as_array())
                                                         : validation_result::success;
                                        log::candidate(candidate, {.result = res,
                                                                   .format = array_spec.format});
                                } else {
                                        log::candidate(candidate);
                                }
                        }

                        continue;
                }

                qDebug() << candidate.path_context.makePath(candidate.applet) << "found!";
                return candidate.node;
        }

        return {};
}
