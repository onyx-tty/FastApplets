// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

// Pull in definitions required by the generics.
#include "PowerApplet/Types/Traits.h"

#include "Core/Applets/Types/Type.h"
#include "Core/Args/Args.h"
#include "Core/Config/AppletConfig/AppletConfig.h"
#include "Core/Config/Locate/Locate.h"
#include "Core/Config/Types/Filepaths.h"
#include "Core/UI/MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QLatin1StringView>
#include <QTimer>
#include <QtGlobal>
#include <CppUtils/Log/QtLog.h>

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

                printHelpMenu(applet::Type::Power);

                QTimer::singleShot(0, &application, &QApplication::quit);
                return application.exec();
        }

        // Find config files
        config::Filepaths applet_filepaths = config::locateFiles(
                applet::toLatin1String(applet::Type::Power));
        const config::Filepaths global_filepaths = config::locateFiles(
                applet::toLatin1String(applet::Type::Global));

        // Inject config filepath and keys filepath if they are valid
        arg::injectArgs(args, applet_filepaths);

        // Config files
        constexpr applet::Type type                = applet::Type::Power;
        const auto [config, _, keys, default_keys] = config::makeAppletConfig<type>(
                applet_filepaths, global_filepaths);

        // GUI
        MainWindow main_window = makeMainWindow(config, keys, default_keys);

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
