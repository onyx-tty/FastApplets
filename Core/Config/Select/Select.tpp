// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Select.h"

#include "Core/Config/Log/Log.h"
#include "Core/Config/Types/Candidates/Candidate.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/NodeView.h"

#include <QDebug>
#include <QtGlobal>
#include <TomlQt/ArrayBounds.h>
#include <TomlQt/TomlQt.h>
#include <concepts>
#include <toml++/toml.hpp>
#include <type_traits>

template<typename TReturn>
requires(tomlqt::TomlQtSupported<std::decay_t<TReturn>>)
const config::Candidate* config::select::candidate(const Candidates& candidates) {
        using DTReturn          = std::decay_t<TReturn>;
        using validation_result = tomlqt::ArrayBounds::validation_result;

        for (const auto& candidate : candidates.get()) {
                bool is_node_valid = false;
                if constexpr (tomlqt::TomlSupported<DTReturn>) {
                        is_node_valid = candidate.node.as<DTReturn>();
                } else { // TODO: Create tomlqt::as instead, repeated tomlqt::value is too heavy
                        is_node_valid = tomlqt::value<DTReturn>(node_view(candidate.node))
                                                .has_value();
                }

                if (is_node_valid) { return &candidate; }
        }

        return nullptr;
}
