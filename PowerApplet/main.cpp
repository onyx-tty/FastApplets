// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

// TODO: Create a template specialization for AppletManager,
//       removing the need for external includes
// Pull in definitions for ConfigManager<PowerApplet> overload
#include "PowerApplet/Types/PowerAppletTraits.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Manager/ConfigManager.h"
#include "Core/Config/Types/ConfigType.h"
#include "Core/UI/MainWindow.h"
#include "Core/UI/PrimaryButtonsFactory.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/PowerCentralWidget.h"

#include <QApplication>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char* argv[]) {
        auto application = QApplication(argc, argv);

        // Setup log formatting
        qt::log::setupLogging();

        // Config files
        using TConfigManager     = ConfigManager<applet::type::power_applet>;
        const auto& config       = TConfigManager::get<config::type::config>();
        const auto& keys         = TConfigManager::get<config::type::keys>();
        const auto& default_keys = TConfigManager::getDefault<config::type::keys>();

        // GUI
        auto* central_widget =
                new PowerCentralWidget(PrimaryButtonsFactory<applet::type::power_applet>::create(
                                               config.getLayoutProperties().getPrimaryButtons(),
                                               config.getPrimaryButtonProperties(),
                                               keys.getPrimaryButton(),
                                               default_keys.getPrimaryButton(), nullptr),
                                       keys.getQuit(),
                                       config.getPrimaryButtonProperties().getDoubleKeyPress(),
                                       nullptr);

        auto main_window = MainWindow(config.getWindowProperties().getTitle(),
                                      config.getWindowProperties().getSize(), central_widget);

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
