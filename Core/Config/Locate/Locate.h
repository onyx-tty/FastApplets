// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLatin1StringView>

namespace config {

class Filepaths;

// Locates config.toml and keys.toml for the given applet.
//
// Empty "applet_name" returns empty Filepaths.
//
// "global" maps to the root FastApplets/ config directory, as the global config
// is stored there.
Filepaths locateFiles(QLatin1StringView applet_name);

} // namespace config
