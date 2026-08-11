// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Types/Candidate.h"
#include "Helpers.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <type_traits>
#include <utility>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>

template<typename T>
requires(std::is_same_v<toml::array, std::decay_t<T>>)
void config::select::helpers::log(const Candidate& candidate, tomlqt::ArrayBounds& arr_bounds,
                                  QStringView arr_format) {
        constexpr bool display_logs = true;
        auto           path         = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        arr_bounds.validate(candidate.node.as_array(), display_logs);

        qWarning() << QString("%1, missing or wrong type! Format: %2")
                              .arg(std::move(path), arr_format.toString());
}

template<typename T>
requires(!std::is_same_v<toml::array, std::decay_t<T>>)
void config::select::helpers::log(const Candidate& candidate) {
        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        qWarning() << QString("%1, missing or wrong type").arg(std::move(path));
}
