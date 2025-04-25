/* Pretty Applets

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

#ifndef POWER_LAYOUT_H
#define POWER_LAYOUT_H

#include "SharedLayout.h"
#include "../Applets/PowerApplet/Widgets/PowerButton.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QString>

#include <array>

struct PowerMainWindowProp : public MainWindowProp {
        const QSize   size;
        const QString title;

        explicit PowerMainWindowProp(const QSize size, const QString title);
};

struct PowerStyleProp : public StyleProp {
        const QString button_stylesheet;

        explicit PowerStyleProp(const QString button_stylesheet);
};

struct PowerButtonProp : public ButtonProp {
        const Qt::Alignment text_alignment;
        const QSize         icon_size;
        const Qt::Alignment icon_alignment;

        explicit PowerButtonProp(const Qt::Alignment text_alignment, const QSize icon_size,
                                 const Qt::Alignment icon_alignment);
};

struct PowerLayoutProp : public LayoutProp {
private:
        // TODO Disable memory deallocation for initialized buttons without also making them
        //	completely immutable (const)
        std::array<PowerButton*, 4>* button_list;

public:
        const QSizePolicy            button_policy;
        const std::array<QIcon, 4>   button_icons;
        const std::array<QString, 4> button_text;

        explicit PowerLayoutProp(const QSizePolicy            button_policy,
                                 const std::array<QIcon, 4>   button_icons,
                                 const std::array<QString, 4> button_text);

        void                               initButtonList(QBoxLayout* layout);
        const std::array<PowerButton*, 4>* getButtonList() const;
        bool                               isSetUp();
};

// TODO Optimize to avoid inheritance of default props
struct PowerLayoutManager : public LayoutManager {
private:
        static PowerLayoutProp layout_prop;

public:
        static const PowerMainWindowProp main_window_prop;
        static const PowerStyleProp      style_prop;
        static const PowerButtonProp     button_prop;

        explicit PowerLayoutManager() = delete;

        static const PowerLayoutProp& getLayoutProp();
        static void                   setup(const QApplication& app, QBoxLayout* layout);
        static bool                   isSetUp();
};

#endif // POWER_LAYOUT_H
