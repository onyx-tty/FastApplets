// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Types/ConfigTomlFiles.h"

#include <string>
#include <QStringLiteral>
#include <QStringView>
#include <QString>

// TODO Test
std::string findFile(QStringView filename, QStringView subdirectory = QStringLiteral(""));

class FileLocator final {
public:
        // Look for configs in $XDG_CONFIG_HOME/FastApplets
        static ConfigTomlFiles locateGlobalConfigFiles();
        static ConfigTomlFiles locatePowerAppletConfigFiles();
};
