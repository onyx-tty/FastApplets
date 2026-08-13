// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Helpers.h"
#include "Core/Config/Select/Types/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <utility>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>

using validation_result = tomlqt::ArrayBounds::validation_result;

void config::select::helpers::log(const Candidate& candidate) {
        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        qWarning() << QString("%1, missing or wrong type").arg(path);
}

void config::select::helpers::log(const Candidate& candidate, validation_result arr_result,
                                  QStringView arr_format) {
        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        tomlqt::logArrayBoundsResult(arr_result);

        qWarning() << QString("%1, missing or wrong type! Format: %2")
                              .arg(std::move(path), arr_format.toString());
}
