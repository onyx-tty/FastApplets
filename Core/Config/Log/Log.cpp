// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Log.h"
#include "Core/Config/Select/Types/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <utility>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>

using validation_result = tomlqt::ArrayBounds::validation_result;

void config::log::candidate(const Candidate& candidate) {
        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        qWarning() << QString("%1, missing or wrong type").arg(path);
}

void config::log::candidate(const Candidate& candidate, ArrayLogSpec array_log_spec) {
        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        tomlqt::logArrayBoundsResult(array_log_spec.result);

        qWarning() << QString("%1, missing or wrong type! Format: %2")
                              .arg(std::move(path), array_log_spec.format.toString());
}
