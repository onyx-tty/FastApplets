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
        auto application = QApplication(argc, argv);

        // Setup log formatting
        qt::log::setupLogging();

        // Config files
        const auto& config       = ConfigManager<applet::type::power_applet>::getConfig();
        const auto& keys         = ConfigManager<applet::type::power_applet>::getKeys();
        const auto& default_keys = ConfigManager<applet::type::power_applet>::getDefaultKeys();

        auto main_window = PowerMainWindow(config, keys, default_keys);

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
