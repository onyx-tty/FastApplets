// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config/Manager/ConfigManager.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/PowerMainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char* argv[]) {
        // Setup log formatting
        qt::log::setupLogging();

        auto application = QApplication(argc, argv);

        // Fetch configs
        ConfigManager::init();

        auto main_window = PowerMainWindow();

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
