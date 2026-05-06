// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ShellRunner.h"

#include <QProcess>
#include <QStringList>

void ShellRunner::runCommand(const QString& command) {
        QStringList parts = QProcess::splitCommand(command);

        QString     program = parts.takeFirst();
        QStringList args    = parts;

        QProcess::startDetached(program, parts);
}
