// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Shell/ShellRunner.h"

#include <QDebug>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QtGlobal>

void ShellRunner::command(QString command) {
        // Avoids splitting empty command into QStringList, which is a fatal QList error
        if (command.isEmpty()) {
                qWarning() << "Passed empty command!";
                return;
        }

        QStringList parts   = QProcess::splitCommand(command);
        QString     program = parts.takeFirst();

        QProcess::startDetached(program, parts);
}
