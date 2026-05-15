// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>
#include <QString>
#include <QStringLiteral>
#include <QStringView>

class ConfigFiles;

// Locates a file under $XDG_CONFIG_HOME/FastApplets/[subdirectory]/filename.
// Returns the absolute path as std::string if found. Otherwise empty string.
std::string findFile(QStringView filename, QStringView subdirectory = QStringLiteral(""));

// Finds files in hardcoded locations.
class FileLocator final {
public:
        // Locates config.toml and keys.toml for the given scope.
        // A scope of "global" maps to the root FastApplets/ directory.
        static ConfigFiles locateConfigFiles(std::string_view scope);
};
