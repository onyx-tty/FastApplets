// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Shell/ShellRunner.h"

#include <utility>
#include <QDebug>
#include <QProcess>
#include <QString>
#include <QtGlobal>

void ShellRunner::command(QString command) {
        // Avoids passing empty command to shell, which does nothing.
        if (command.isEmpty()) {
                qWarning() << "Passed empty command!";
                return;
        }

        QProcess::startDetached("/bin/sh", {"-c", std::move(command)});
}
