// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

// TODO: Create a template specialization for AppletManager,
//       removing the need for external includes
// Pull in definitions for ConfigManager<ActionApplet> overload
#include "ActionApplet/Types/ActionAppletTraits.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Manager/ConfigManager.h"
#include "Core/Config/Types/ConfigType.h"
#include "Core/UI/PrimaryButtonsFactory.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/ActionMainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char* argv[]) {
        auto application = QApplication(argc, argv);

        // Setup log formatting
        qt::log::setupLogging();

        // Config files
        using TConfigManager     = ConfigManager<applet::type::action_applet>;
        const auto& config       = TConfigManager::get<config::type::config>();
        const auto& keys         = TConfigManager::get<config::type::keys>();
        const auto& default_keys = TConfigManager::getDefault<config::type::keys>();

        auto main_window = ActionMainWindow(
                config.getWindowProperties().getTitle(), config.getWindowProperties().getSize(),
                PrimaryButtonsFactory<applet::type::action_applet>::create(config, keys,
                                                                           default_keys, nullptr),
                keys.getQuit(), config.getPrimaryButtonProperties().getDoubleKeyPress());

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
