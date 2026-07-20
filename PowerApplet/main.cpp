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
#include "CppUtils/Log/QtLog.h"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QLatin1StringView>
#include <QTimer>
#include <QtGlobal>
#include <QStringView>

namespace {

// Inject config filepath and keys filepath if they are valid
// TODO: This is a workaround. Ideally, valid args should be assigned prior to config::locateFiles
//       lookups. Currently that's not possible without collapsing config::locateFiles
//       and separating the lookups of config and keys; paths for each have to be injected
//       in separation for that to work
void injectArgs(arg::CmdArgs& args, config::Filepaths& filepaths) {
        const auto injector = [](QString& target, QString& source, QStringView source_name) {
                QFileInfo file = QFileInfo(source);
                if (file.exists() && file.isFile() && file.isReadable()) {
                        qDebug() << QString("Found file %1, it was passed as a %2 cmd argument, injecting")
                                            .arg(source).arg(source_name);
                        target = source;
                } else if (!source.isEmpty()) {
                        qWarning()
                                << QString("File %1 not found, it was passed as a %2 cmd argument")
                                           .arg(source).arg(source_name);
                }
        };

        injector(filepaths.config, args.config_path, u"config");
        injector(filepaths.keys, args.keys_path, u"keys");
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

                printHelpMenu(applet::type::power);

                QTimer::singleShot(0, &application, &QApplication::quit);
                return application.exec();
        }

        // Find config files
        config::Filepaths applet_filepaths = config::locateFiles(
                applet::toLatin1String(applet::type::power));
        const config::Filepaths global_filepaths = config::locateFiles(
                applet::toLatin1String(applet::type::global));

        // Inject config filepath and keys filepath if they are valid
        injectArgs(args, applet_filepaths);

        // Config files
        constexpr applet::type type = applet::type::power;
        const auto [config, _, keys,
                    default_keys]   = config::makeAppletConfigTuple<type>(applet_filepaths,
                                                                          global_filepaths);

        // GUI
        MainWindow main_window = makeMainWindow(config, keys, default_keys);

        // Print application info
        qInfo() << "Applet resolution:" << main_window.size();
        qDebug() << "Qt Version:" << qVersion() << "\n";

        main_window.show();

        // Start the Qt event loop
        return application.exec();
}
