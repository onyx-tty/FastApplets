// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Shell.h"
#include "Core/Shell/Types/ShellContext.h"

#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QtGlobal>
#include <utility>

void runCommand(QString command, ShellContext context) {
        // Avoids passing empty command to shell, which does nothing.
        if (command.isEmpty()) {
                qWarning() << "Passed empty command!";
                return;
        }

        auto* process = new QProcess();
        QProcess::connect(process, &QProcess::finished, [process, context]() {
                qInfo() << "Command dispatched, quitting...";

                process->deleteLater();

                if (context.terminate_on_command_exit) {
                        // Let the event loop process all pending events before quitting.
                        // This allows all logs to be printed out before quitting.
                        QTimer::singleShot(0, []() { QCoreApplication::quit(); });
                }
        });

        process->start("/bin/sh", {"-c", std::move(command)});
}
