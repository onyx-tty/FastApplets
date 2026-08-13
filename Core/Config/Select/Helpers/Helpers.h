// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Select/Types/Candidate.h"

#include <TomlQt/ArrayBounds.h>
#include <toml++/toml.hpp>
#include <QStringView>

namespace config::select::helpers {

using validation_result = tomlqt::ArrayBounds::validation_result;

void log(const Candidate& candidate);
void log(const Candidate& candidate, validation_result arr_result, QStringView arr_format);

} // namespace config::select::helpers
