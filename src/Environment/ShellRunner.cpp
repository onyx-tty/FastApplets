/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "ShellRunner.h"
#include "ShellCommand.h"

#include <QProcess>

// TODO Replace to eliminate command injection caused by shell interpretation

void ShellRunner::runShutdownCommand(const ShellCommand& command) {
        QProcess::startDetached(command.program, command.arguments);
}

void ShellRunner::runRebootCommand(const ShellCommand& command) {
        QProcess::startDetached(command.program, command.arguments);
}

void ShellRunner::runSuspendCommand(const ShellCommand& command) {
        QProcess::startDetached(command.program, command.arguments);
}

void ShellRunner::runHibernateCommand(const ShellCommand& command) {
        QProcess::startDetached(command.program, command.arguments);
}
