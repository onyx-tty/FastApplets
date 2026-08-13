// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Types/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <QStringView>

namespace config::log {

using config::select::Candidate;
using validation_result = tomlqt::ArrayBounds::validation_result;

struct ArrayLogSpec final {
        validation_result result;
        QStringView       format;
};

void candidate(const Candidate& candidate);
void candidate(const Candidate& candidate, ArrayLogSpec array_log_spec);

} // namespace config::log
