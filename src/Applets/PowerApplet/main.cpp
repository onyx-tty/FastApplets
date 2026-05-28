// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CppUtils/Log/QtLog.h"
#include "UI/PowerMainWindow.h"

#include <QApplication>

// Qt 6.5+ provides dedicated headers <QtLogging> and <QtVersion>.
// Use them when possible to reduce compilation times.
// For older Qt versions, fall back to the full <QtCore>.
// <QtVersionChecks> didn't exist before Qt 6.5, so it can't be used either.
#if __has_include(<QtLogging>) // Qt >=6.5
#       include <QtLogging>
#       include <QtVersion>
#else
#       include <QtGlobal> // Qt <6.5
#endif

int main(int argc, char* argv[]) {
        // Setup log formatting
        qt::log::setupLogging();

        QApplication application(argc, argv);
        PowerMainWindow main_window;

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
