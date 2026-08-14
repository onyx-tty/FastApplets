// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Log.h"
#include "Core/Config/Types/Candidates/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <utility>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>

using validation_result = tomlqt::ArrayBounds::validation_result;

void config::log::candidate(const Candidate& candidate, bool success) {
        if (candidate.quiet) { return; }

        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        if (success) {
                qDebug() << path << "found!";
        } else {
                qWarning() << QString("%1, missing or wrong type").arg(path);
        }
}

void config::log::candidate(const Candidate& candidate, bool success, ArrayLogSpec array_log_spec) {
        using result = tomlqt::ArrayBounds::validation_result;

        if (candidate.quiet) { return; }

        auto path = candidate.path_context.makePath(candidate.applet);

        if (path.isNull()) { qFatal("Passed null path"); }

        tomlqt::logArrayBoundsResult(array_log_spec.result);

        if (success && array_log_spec.result == result::success) {
                qDebug() << path << "found!";
        } else {
                qWarning() << QString("%1, missing or wrong type! Format: %2")
                                      .arg(std::move(path), array_log_spec.format.toString());
        }
}
