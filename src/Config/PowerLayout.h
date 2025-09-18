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

#ifndef POWER_LAYOUT_H
#define POWER_LAYOUT_H

#include "SharedLayout.h"
#include "../Applets/PowerApplet/Widgets/PowerButton.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QString>

struct PowerMainWindowProp : public MainWindowProp {
        const QSize   size;
        const QString title;

        explicit PowerMainWindowProp();
};

struct PowerStyleProp : public StyleProp {
        const QString selected;
        const QString unselected;

        explicit PowerStyleProp();
};

struct PowerButtonProp : public ButtonProp {
        const Qt::Alignment text_alignment;
        const QSize         icon_size;
        const Qt::Alignment icon_alignment;

        explicit PowerButtonProp();
};

struct PowerLayoutProp : public LayoutProp {
        const QSizePolicy            button_policy;
        const std::array<QIcon, 4>   button_icons;
        const std::array<QString, 4> button_text;

        explicit PowerLayoutProp();

        std::array<PowerButton*, 4>& buttonListSingleton(QWidget* parent, QHBoxLayout* layout,
                                                         bool&& is_instantiated);
};

/* Actual Layout Manager */
struct PowerLayoutManager : public LayoutManager {
        static MainWindowProp  main_window_prop;
        static StyleProp       style_prop;
        static ButtonProp      button_prop;
        static PowerLayoutProp layout_prop;

        explicit PowerLayoutManager();
};

#endif // POWER_LAYOUT_H
