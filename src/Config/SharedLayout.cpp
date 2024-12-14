/* Pretty Applets

   Copyright (C) 2024 Łukasz Wrodarczyk

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

#include "SharedLayout.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QString>

/* These are global settings shared by applets. Modify below to adjust their application style. */

MainWindowProp::MainWindowProp() : size(QSize(960, 220)), title("test_window") {};

StyleProp::StyleProp() :
        selected("text-align: center top; background-color: lightblue; border: 2px solid blue;"),
        unselected("text-align: center top;") {};

// TODO Text size
ButtonProp::ButtonProp() :
        text_alignment(Qt::Alignment(Qt::AlignHCenter | Qt::AlignTop)), icon_size(QSize(64, 64)),
        icon_alignment(Qt::AlignHCenter | Qt::AlignCenter) {};

LayoutProp::LayoutProp() :
        button_policy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)) {};

QString& EnvProp::getProjectRoot(QApplication& app, const QString& marker) {
        static QString project_root;

        if (!project_root.isEmpty()) { // TODO This is too nested, clean it up
                return project_root;
        }

        QStringList search_paths = {app.applicationDirPath(), QDir::currentPath()};
        for (const QString& start_directory : search_paths) {
                qInfo() << "Searching paths...";
                QDir directory(start_directory);
                while (!directory.isRoot()) {
                        qInfo() << "Current directory:" << directory.filesystemAbsolutePath();
                        if (QFileInfo::exists((directory.filePath(marker)))) {
                                qInfo() << "Match!";
                                project_root = directory.absolutePath();
                                return project_root;
                        } else {
                                directory.cdUp();
                        }
                }
        }
        // if not found
        if (project_root.isEmpty()) {
                qFatal("Fatal! Project root directory not located in locateProjectRoot()!");
                return project_root; // will never occur because of termination
        }
}

EnvProp::EnvProp() {};

LayoutManager::LayoutManager() {};

MainWindowProp LayoutManager::main_window_prop;
StyleProp      LayoutManager::style_prop;
ButtonProp     LayoutManager::button_prop;
LayoutProp     LayoutManager::layout_prop;
EnvProp        LayoutManager::env_prop;
