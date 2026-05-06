// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletTypes.h"
#include "Config/Types/ConfigTomlFiles.h"

#include <string>
#include <QString>
#include <QStringLiteral>
#include <QStringView>

// TODO Test
std::string findFile(QStringView filename, QStringView subdirectory = QStringLiteral(""));

class FileLocator final {
public:
        // Look for configs in $XDG_CONFIG_HOME/FastApplets
        static ConfigTomlFiles locateConfigFiles(std::string_view scope);
};
