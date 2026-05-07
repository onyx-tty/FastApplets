// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ShellRunner.h"

#include <QProcess>
#include <QStringList>

// QProcess::splitCommand treats single quotes as regular characters.
// Treat them like proper quotes by reinterpreting single quotes as double quotes.
inline QString singleQuotesToDoubleQuotes(QString command) {
        command.replace("'", "\"");

        return command;
}

void ShellRunner::runCommand(const QString& command) {
        QString     reinterpreted = singleQuotesToDoubleQuotes(command);

        QStringList parts         = QProcess::splitCommand(reinterpreted);

        QString     program = parts.takeFirst();

        QProcess::startDetached(program, parts);
}
