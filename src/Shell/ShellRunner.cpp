// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ShellRunner.h"

#include <QProcess>
#include <QString>
#include <QStringList>

void ShellRunner::runCommand(QString command) {
        // QProcess::splitCommand treats single quotes as regular characters
        // Treat them like proper quotes by reinterpreting single quotes as double quotes
        command.replace("'", "\"");

        QStringList parts   = QProcess::splitCommand(command);
        QString     program = parts.takeFirst();

        QProcess::startDetached(program, parts);
}
