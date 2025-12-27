/* FastApplets

   Copyright (C) 2025 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "Core/Log.h"
#include "RootResolver.h"

#include <QApplication>
#include <QDir>
#include <QStringList>

RootResolver::RootResolver(const QString& project_root_marker) {
        if (!QApplication::instance()) {
                QDEBUG() << "QApplication not instantiated yet!";
        }
        QStringList search_paths = {QApplication::applicationDirPath(), QDir::currentPath()};
        QDEBUG() << "Determining project root...";
        for (const QString& start_directory : search_paths) {
                QDEBUG() << "Searching paths...";
                QDir directory(start_directory);
                while (!directory.isRoot()) {
                        QDEBUG() << "Current directory:" << directory.filesystemAbsolutePath();
                        if (QFileInfo::exists((directory.filePath(project_root_marker)))) {
                                QDEBUG() << "Match for" << project_root_marker << "!";
                                project_root = directory.absolutePath();
                                break;
                        } else {
                                directory.cdUp();
                        }
                }
        }
}

RootResolver& RootResolver::getInstance() {
        static RootResolver root_resolver("README.md");
        return root_resolver;
}

const QString& RootResolver::getProjectRoot() {
        return project_root;
}
