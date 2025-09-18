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

#ifndef SHARED_LAYOUT_H
#define SHARED_LAYOUT_H

#include <QIcon>
#include <QSize>
#include <QSizePolicy>

class PowerButton; // external

struct MainWindowProp {
        const QSize   size;
        const QString title;

        explicit MainWindowProp();
};

struct StyleProp {
        const QString selected;
        const QString unselected;

        explicit StyleProp();
};

struct ButtonProp {
        const Qt::Alignment          text_alignment;
        const QSize                  icon_size;
        const Qt::Alignment          icon_alignment;

        explicit ButtonProp();
};

struct LayoutProp {
        const QSizePolicy button_policy;

        explicit LayoutProp();
};

/* Actual Layout Manager */
struct LayoutManager {
        static MainWindowProp main_window_prop;
        static StyleProp      style_prop;
        static ButtonProp     button_prop;
        static LayoutProp     layout_prop;

        explicit LayoutManager();
};

#endif // SHARED_LAYOUT_H
