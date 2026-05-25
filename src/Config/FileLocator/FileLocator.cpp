// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FileLocator.h"
#include "Config/Types/ConfigFiles.h"

#include <string>
#include <string_view>
#include <QFileInfo>
#include <QString>
#include <QStringLiteral>
#include <QStringView>

// Qt 6.5+ provides a dedicated <QtEnvironmentVariables> header for environment
// variable utilities. Use it when possible to reduce compilation times.
// For older Qt versions, fall back to the full <QtCore>.
// <QtVersionChecks> didn't exist before Qt 6.5, so it can't be used either.
#if __has_include(<QtEnvironmentVariables>)
        #include <QtEnvironmentVariables> // Qt >=6.5
#else
        #include <QtGlobal> // Qt <6.5
#endif

std::string findFile(QStringView filename, QStringView subdirectory) {
        QString subdir   = subdirectory.empty() ? subdirectory.toString()
                                                : subdirectory.toString() + "/";
        QString filepath = qEnvironmentVariable("XDG_CONFIG_HOME") + "/FastApplets/" + subdir
                         + filename.toString();

        // Returns empty string if file doesn't exist to avoid downstream errors,
        // like parsing the wrong file by accident.
        if (!QFileInfo::exists(filepath)) { return {}; }

        return filepath.toStdString();
}

ConfigFiles FileLocator::configFiles(std::string_view applet_name) {
        ConfigFiles files{};

        // Global config is in root, not in a separate directory. Redirect to root.
        if (applet_name == "global") { applet_name = ""; }

        files.config = findFile(QStringLiteral("config.toml"),
                                QString::fromStdString(std::string{applet_name}));
        files.keys   = findFile(QStringLiteral("keys.toml"),
                                QString::fromStdString(std::string{applet_name}));

        return files;
}
