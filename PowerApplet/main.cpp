// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

// TODO: Create a template specialization for AppletManager,
//       removing the need for external includes
// Pull in definitions for ConfigManager<PowerApplet> overload
#include "PowerApplet/Types/PowerAppletTraits.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/FileLocator/FileLocator.h"
#include "Core/Config/Manager/ConfigManager.h"
#include "Core/Config/Types/ConfigFilepaths.h"
#include "Core/Config/Types/ConfigType.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/MainWindow.h"
#include "Core/UI/PrimaryButtonsFactory.h"
#include "CppUtils/Log/QtLog.h"

#include <QApplication>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char* argv[]) {
        auto application = QApplication(argc, argv);

        // Setup log formatting
        qt::log::setupLogging();

        // Find config files
        const ConfigFilepaths applet_filepaths = FileLocator::configFiles(
                applet::toLatin1String(applet::type::power_applet));
        const ConfigFilepaths global_filepaths = FileLocator::configFiles(
                applet::toLatin1String(applet::type::global));

        // Config files
        using TConfigManager = ConfigManager<applet::type::power_applet>;
        TConfigManager::setup(applet_filepaths, global_filepaths);
        const auto& config       = TConfigManager::get<config::type::config>({.defaults = false});
        const auto& keys         = TConfigManager::get<config::type::keys>({.defaults = false});
        const auto& default_keys = TConfigManager::get<config::type::keys>({.defaults = true});

        // GUI
        auto* central_widget = new CentralWidget(
                PrimaryButtonsFactory::create(config.getLayoutProperties().getPrimaryButtons(),
                                              config.getPrimaryButtonProperties(),
                                              keys.getPrimaryButtons(),
                                              default_keys.getPrimaryButtons(), nullptr),
                keys.getQuit(), config.getPrimaryButtonProperties().getDoubleKeyPress(), nullptr);

        auto main_window = MainWindow(config.getWindowProperties().getTitle(),
                                      config.getWindowProperties().getSize(), central_widget);

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
