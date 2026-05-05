// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Types/ConfigTomlFiles.h"

class FileLocator final {
public:
        static ConfigTomlFiles locateGlobalConfigFiles();
        static ConfigTomlFiles locatePowerAppletConfigFiles();
};
