// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>

class ConfigFilepaths;

// Finds files in hardcoded locations.
class FileLocator final {
public:
        // Locates config.toml and keys.toml for the given applet.
        // "global" maps to the root FastApplets/ config directory, as the global config
        // is stored there.
        static ConfigFilepaths configFiles(std::string_view applet_name);
};
