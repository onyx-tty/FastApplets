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
#include <QPalette>
#include <QSize>
#include <QSizePolicy>
#include <QString>

/* These are global settings shared by applets. */
/* Modify below to adjust their application style. */
QString              project_root_marker = "README.md";
/* LayoutManager */
const MainWindowProp LayoutManager::main_window_prop(QSize(960, 220), "test_window");
const StyleProp      LayoutManager::style_prop("text-align: center top;");
const ButtonProp     LayoutManager::button_prop(Qt::Alignment(Qt::AlignHCenter | Qt::AlignTop),
                                                QSize(64, 64),
                                                Qt::Alignment(Qt::AlignHCenter | Qt::AlignCenter));
const LayoutProp     LayoutManager::layout_prop(QSizePolicy(QSizePolicy::Expanding,
                                                            QSizePolicy::Expanding));
/* END CONFIG */
const EnvProp&       LayoutManager::getEnvProp() {
        return env_prop;
}

void LayoutManager::setup(const QApplication& app) {
        env_prop = getEnvProp();
        env_prop.initProjectEnvironment(app, project_root_marker);
}

bool LayoutManager::isSetUp() {
        return getEnvProp().isSetUp();
}

/* MainWindowProp */
MainWindowProp::MainWindowProp(const QSize size, const QString title) : size(size), title(title) {};

/* StyleProp */
StyleProp::StyleProp(const QString button_stylesheet) : button_stylesheet(button_stylesheet) {};

/* ButtonProp */
ButtonProp::ButtonProp(const Qt::Alignment text_alignment, const QSize icon_size,
                       const Qt::Alignment icon_alignment) :
        text_alignment(text_alignment), icon_size(icon_size), icon_alignment(icon_alignment) {};

/* LayoutProp */
LayoutProp::LayoutProp(const QSizePolicy button_policy) : button_policy(button_policy) {};

/* EnvProp */
EnvProp::EnvProp() {};

void EnvProp::initProjectEnvironment(const QApplication& app, const QString project_root_marker) {
        QStringList search_paths = {app.applicationDirPath(), QDir::currentPath()};
        qDebug() << "Determining project root...";
        for (const QString& start_directory : search_paths) {
                qDebug() << "Searching paths...";
                QDir directory(start_directory);
                while (!directory.isRoot()) {
                        qDebug() << "Current directory:" << directory.filesystemAbsolutePath();
                        if (QFileInfo::exists((directory.filePath(project_root_marker)))) {
                                qDebug() << "Match for" << project_root_marker << "!";
                                project_root = directory.absolutePath();
                                break;
                        } else {
                                directory.cdUp();
                        }
                }
        }
        dotenv_filepath = project_root + "/src/Config/.env";

        // if not set up correctly
        if (!isSetUp()) {
                qFatal("Failed to determine project root! Impossible to proceed reliably!");
                QApplication::quit();
        }
}

const QString& EnvProp::getDotenvFilepath() const {
        if (!isSetUp()) {
                qFatal("Failed to locate project root! The program wasn't set up correctly!");
                QApplication::quit();
        }

        return dotenv_filepath;
}

bool EnvProp::isSetUp() const {
        return (!project_root.isEmpty() || !dotenv_filepath.isEmpty());
}
