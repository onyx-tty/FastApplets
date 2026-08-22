// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Validate.h"

#include "Core/Config/Types/ErrorType.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/Config/Validate/Validate.h"

#include <QDebug>
#include <QtGlobal>
#include <TomlQt/ArrayBounds.h>
#include <TomlQt/TomlQt.h>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>

template<typename TExpected>
std::optional<config::ErrorType> config::validate::node(node_view view) {
        using DTExpected = std::decay_t<TExpected>;

        if (!view) { return ErrorType::Missing; }

        if constexpr (!tomlqt::TomlQtSupported<DTExpected>) { return ErrorType::InvalidType; }

        return std::nullopt; // Success
}

template<typename TExpected>
requires(std::is_same_v<TExpected, toml::array>)
std::optional<config::ErrorType> config::validate::node(
        node_view view, const ArrayBounds& array_bounds) {
        using validation_result = tomlqt::ArrayBounds::validation_result;

        auto error = node<toml::array>(view);
        if (error) { return error; }

        auto valid = (array_bounds.validate(view.as_array()) == validation_result::success);

        if (!valid) { return ErrorType::ArrayOutOfBounds; }

        return std::nullopt; // Success
}
