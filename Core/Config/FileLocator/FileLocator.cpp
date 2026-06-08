// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FileLocator.h"
#include "Core/Config/Types/ConfigFilepaths.h"

#include <string_view>
#include <QFileInfo>
#include <QString>
#include <QStringView>
#include <QtGlobal>

ConfigFilepaths FileLocator::configFiles(std::string_view applet_name) {
        ConfigFilepaths filepaths = {};

        // Global config is in root, not in a separate directory. Redirect to root.
        if (applet_name == "global") { applet_name = ""; }

        QStringView applet_path = QString("%1/FastApplets/%2%3")
                                          .arg(qEnvironmentVariable("XDG_CONFIG_HOME"),
                                               QString::fromStdString(std::string(applet_name)),
                                               applet_name.empty() ? "" : "/");

        filepaths.config = QString("%1%2").arg(applet_path, u"config.toml").toStdString();
        filepaths.keys   = QString("%1%2").arg(applet_path, u"keys.toml").toStdString();

        // Validates filepaths, replaces with empty string if invalid
        // TODO: Make ConfigFilepaths store paths as QString to remove the
        //       QString::fromStdString() workaround
        if (!QFileInfo::exists(QString::fromStdString(filepaths.config))) { filepaths.config = {}; }
        if (!QFileInfo::exists(QString::fromStdString(filepaths.keys))) { filepaths.keys = {}; }

        return filepaths;
}
