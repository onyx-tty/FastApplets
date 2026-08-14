// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Candidates/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <QStringView>

namespace config {
class Candidate;
} // namespace config

namespace config::log {

using config::Candidate;
using validation_result = tomlqt::ArrayBounds::validation_result;

struct ArrayLogSpec final {
        validation_result result = validation_result::null_ptr;
        QStringView       format = u"";
};

void candidate(const Candidate& candidate);
void candidate(const Candidate& candidate, ArrayLogSpec array_log_spec);

} // namespace config::log
