// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

// TODO: Create a template specialization for AppletManager,
//       removing the need for external includes
// Pull in definitions for ConfigManager<PowerApplet> overload
#include "PowerApplet/Types/PowerAppletTraits.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Args/Args.h"
#include "Core/Config/FileLocator/FileLocator.h"
#include "Core/Config/Manager/ConfigManager.h"
#include "Core/Config/Types/ConfigFilepaths.h"
#include "Core/UI/MainWindow.h"
#include "CppUtils/Log/QtLog.h"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QLatin1StringView>
#include <QTimer>
#include <QtGlobal>

namespace {

// Inject config filepath and keys filepath if they are valid
// TODO: Collapse into separate functions
void injectArgs(arg::CmdArgs& args, config::Filepaths& filepaths) {
        // TODO: This is a workaround. Ideally, valid args should be assigned prior to FileLocator
        //       lookups. Currently that's not possible without collapsing FileLocator::configFiles
        //       and separating the lookups of config and keys; paths for each have to be injected
        //       in separation for that to work
        // TODO: Create a validator class
        QFileInfo file = QFileInfo(args.config_path);
        if (file.exists() && file.isFile() && file.isReadable()) {
                qDebug() << QString("Found file %1, it was passed as a config cmd argument, injecting")
                                    .arg(args.config_path);
                filepaths.config = args.config_path;
        } else if (!args.config_path.isEmpty()) {
                qWarning() << "File" << args.config_path
                           << "not found, it was passed as a config cmd argument";
        }

        file = QFileInfo(args.keys_path);
        if (file.exists() && file.isFile() && file.isReadable()) {
                qDebug() << QString("Found file %1, it was passed as a keys cmd argument, injecting")
                                    .arg(args.keys_path);
                filepaths.keys = args.keys_path;
        } else if (!args.keys_path.isEmpty()) {
                qWarning() << "File" << args.keys_path
                           << "not found, it was passed as a keys cmd argument";
        }
}

} // namespace

int main(int argc, char* argv[]) {
        auto application = QApplication(argc, argv);

        // Setup log formatting
        qt::log::setupLogging();

        // Parse args
        arg::CmdArgs args = {};
        try {
                printArgs(argc, argv);
                args = arg::parse(argc, argv);
        } catch (const HelpMenuRequested& e) {
                QLatin1StringView msg(e.what());
                if (!msg.empty()) { qWarning() << msg; }

                printHelpMenu(applet::type::power_applet);

                QTimer::singleShot(0, &application, &QApplication::quit);
                return application.exec();
        }

        // Find config files
        config::Filepaths applet_filepaths = FileLocator::configFiles(
                applet::toLatin1String(applet::type::power_applet));
        const config::Filepaths global_filepaths = FileLocator::configFiles(
                applet::toLatin1String(applet::type::global));

        // Inject config filepath and keys filepath if they are valid
        injectArgs(args, applet_filepaths);

        // Config files
        using TConfigManager = ConfigManager<applet::type::power_applet>;
        TConfigManager::setup(applet_filepaths, global_filepaths);
        const auto& [config, keys, default_keys] = TConfigManager::getAll();

        // GUI
        MainWindow main_window = makeMainWindow(config, keys, default_keys);

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
