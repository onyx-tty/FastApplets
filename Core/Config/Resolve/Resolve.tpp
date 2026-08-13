// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Select.h"
#include "Core/Config/Select/Types/Candidates.h"
#include "Core/Config/Types/ArraySpec.h"
#include "Resolve.h"

#include <TomlQt/TomlQt.h>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <QStringView>

template<typename T>
requires(config::resolve::ReturnByValue<T>)
std::optional<T> config::resolve::from(const Candidates& candidates) {
        using DT = std::decay_t<T>;

        auto result = config::select::candidate<DT>(candidates);

        return tomlqt::value<DT>(result);
}

template<typename T>
requires(config::resolve::ReturnByView<T>)
const T* config::resolve::from(const Candidates& candidates, const ArraySpec& array_spec) {
        using DT = std::decay_t<T>;

        auto result = config::select::candidate<DT>(candidates, array_spec);

        return result.template as<DT>();
}
