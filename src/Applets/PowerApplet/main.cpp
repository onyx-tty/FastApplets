// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CppUtils/Log/QtLog.h"
#include "UI/PowerMainWindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
        // Initialize QApplication
        QApplication application(argc, argv);

        // Create main window
        PowerMainWindow main_window;

        // Print information
        QINFO() << "Applet resolution:" << main_window.size();
        QDEBUG() << "Qt Version:" << qVersion();
        qInfo() << " ";

        // Show main window
        main_window.show();

        // Start the Qt event loop and return its exit code upon termination
        return application.exec();
}
