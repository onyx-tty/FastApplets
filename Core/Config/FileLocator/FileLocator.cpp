// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FileLocator.h"
#include "Core/Config/Types/ConfigFilepaths.h"

#include <QFileInfo>
#include <QLatin1StringView>
#include <QString>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

ConfigFilepaths FileLocator::configFiles(QLatin1StringView applet_name) {
        using namespace Qt::StringLiterals;

        ConfigFilepaths filepaths = {};

        // Global config is in root, not in a separate directory. Redirect to root.
        if (applet_name == "global") { applet_name = ""_L1; }

        QString applet_path = QString("%1/FastApplets/%2%3")
                                      .arg(qEnvironmentVariable("XDG_CONFIG_HOME"),
                                           applet_name.toString(),
                                           applet_name.isEmpty() ? u"" : u"/");

        filepaths.config = QString("%1%2").arg(applet_path, u"config.toml");
        filepaths.keys   = QString("%1%2").arg(applet_path, u"keys.toml");

        // Validates filepaths, replaces with empty string if invalid
        if (!QFileInfo::exists(filepaths.config)) {
                qWarning() << "File" << filepaths.config << "not found";
                filepaths.config = {};
        }

        if (!QFileInfo::exists(filepaths.keys)) {
                qWarning() << "File" << filepaths.keys << "not found";
                filepaths.keys = {};
        }

        return filepaths;
}
