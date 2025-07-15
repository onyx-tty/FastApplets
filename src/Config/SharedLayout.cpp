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

#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QString>

QSize setMainWindowSize() {
        return QSize(960, 220);
}

QString setMainWindowTitle() {
        return QString("test_window");
}

QString setStyleSelected() {
        return QString(
                "text-align: center top; background-color: lightblue; border: 2px solid blue;");
}

QString setStyleUnselected() {
        return QString("text-align: center top;");
}

Qt::Alignment setButtonTextAlignment() {
        return Qt::Alignment(Qt::AlignHCenter | Qt::AlignBottom);
}

Qt::Alignment setButtonIconAlignment() {
        return Qt::Alignment(Qt::AlignHCenter | Qt::AlignTop);
}

QSize setButtonIconSize() {
        return QSize(64, 64);
}

QSizePolicy setButtonSizePolicy() {
        return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindowProp::MainWindowProp() : size(setMainWindowSize()), title(setMainWindowTitle()) {};

StyleProp::StyleProp() : selected(setStyleSelected()), unselected(setStyleUnselected()) {};

// TODO Text size
ButtonProp::ButtonProp() :
        text_alignment(setButtonTextAlignment()), icon_size(setButtonIconSize()),
        icon_alignment(setButtonIconAlignment()) {};

LayoutProp::LayoutProp(QSizePolicy button_policy) : button_policy(button_policy) {};

LayoutManager::LayoutManager() {};

/* Customize here */
MainWindowProp LayoutManager::main_window_prop;
StyleProp      LayoutManager::style_prop;
ButtonProp     LayoutManager::button_prop;
LayoutProp     LayoutManager::layout_prop(setButtonSizePolicy());
