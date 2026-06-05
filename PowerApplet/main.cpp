// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

// TODO: Create a template specialization for AppletManager,
//       removing the need for external includes
// Pull in definitions for ConfigManager<PowerApplet> overload
#include "PowerApplet/Types/PowerAppletTraits.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Manager/ConfigManager.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/PowerMainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char* argv[]) {
        // Setup log formatting
        qt::log::setupLogging();

        auto application = QApplication(argc, argv);

        const auto& config      = ConfigManager<applet::type::power_applet>::getConfig();
        auto        main_window = PowerMainWindow(config.getWindowProperties());

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
