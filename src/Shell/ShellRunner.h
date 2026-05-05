// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ShellCommand.h"

class ShellRunner final {
public:
        ShellRunner() = delete;
        static void runCommand(const ShellCommand& command);
};
