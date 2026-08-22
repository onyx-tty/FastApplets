// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "View.h"
#include "Core/Config/Types/ErrorType.h"
#include "Core/Config/Validate/Validate.h"

#include <QDebug>
#include <QtGlobal>
#include <TomlQt/TomlQt.h>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <utility>

template<typename T>
requires(!std::is_same_v<T, toml::array>)
std::optional<T> config::ConfigView::resolve() const {
        std::optional<config::ErrorType> error_type = {};

        for (auto source : sources) {
                error_type = config::validate::node<T>(source);

                if (error_type) { continue; }

                qDebug().noquote() << QString("(%1) %2 %3").arg(filename, "Found", path);

                return tomlqt::value<T>(source);
        }

        // This should never happen, as the previous branch always returns on !error_type.
        // The log message remains in case of future refactors.
        if (!error_type) {
                qCritical() << "Expected error_type has not been found."
                            << "This branch should never execute";
                std::unreachable();
        }

        config::printError(error_type.value(), path, filename);

        return std::nullopt;
}

template<typename T>
requires(std::is_same_v<T, toml::array>)
const toml::array* config::ConfigView::resolve(const ArrayBounds& array_bounds) const {
        std::optional<config::ErrorType> error_type = {};

        for (auto source : sources) {
                error_type = config::validate::node<toml::array>(source, array_bounds);

                if (error_type) { continue; }

                qDebug().noquote() << QString("(%1) %2 %3").arg(filename, "Found", path);

                return source.as_array();
        }

        // This should never happen, as the previous branch always returns on !error_type.
        // The log message remains in case of future refactors.
        if (!error_type) {
                qCritical() << "Expected error_type has not been found."
                            << "This branch should never execute";
                std::unreachable();
        }

        config::printError(error_type.value(), path, filename);

        return nullptr;
}
