// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Shell/ShellRunner.h"

#include <QProcess>
#include <QString>
#include <QStringList>

// TODO: Quit and warn if empty
// FIXME: Splitting command into QStringList is a fatal QList error if command is empty!
void ShellRunner::command(QString command) {
        // QProcess::splitCommand treats single quotes as regular characters
        // Treat them like proper quotes by reinterpreting single quotes as double quotes
        command.replace("'", "\"");

        QStringList parts   = QProcess::splitCommand(command);
        QString     program = parts.takeFirst();

        QProcess::startDetached(program, parts);
}
