// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ShellRunner.h"
#include "ShellCommand.h"

#include <QProcess>

// TODO Replace to eliminate command injection caused by shell interpretation

void ShellRunner::runCommand(const ShellCommand& command) {
        QProcess::startDetached(command.program, command.arguments);
}
