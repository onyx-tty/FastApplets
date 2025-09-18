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

#include <QApplication>
#include <QIcon>
#include <QSize>
#include <QSizePolicy>

class PowerButton; // forward declaration

struct MainWindowProp {
        const QSize   size;
        const QString title;

        explicit MainWindowProp(const QSize size, const QString title);
};

struct StyleProp {
        const QString button_stylesheet;

        explicit StyleProp(const QString button_stylesheet);
};

struct ButtonProp {
        // TODO Text size
        const Qt::Alignment text_alignment;
        const QSize         icon_size;
        const Qt::Alignment icon_alignment;

        explicit ButtonProp(const Qt::Alignment text_alignment, const QSize icon_size,
                            const Qt::Alignment icon_alignment);
};

struct LayoutProp {
        const QSizePolicy button_policy;

        explicit LayoutProp(const QSizePolicy button_policy);
};

struct EnvProp {
protected:
        QString project_root;
        QString dotenv_filepath;

public:
        explicit EnvProp();

        // TODO Error handling for incorrect project root
        void initProjectEnvironment(const QApplication& app, const QString project_root_marker);
        const QString& getDotenvFilepath() const;
        bool           isSetUp() const;
};

/* Actual Layout Manager */
// TODO Polymorphism for further customization
struct LayoutManager {
private:
        static inline EnvProp env_prop;

public:
        static const MainWindowProp main_window_prop;
        static const StyleProp      style_prop;
        static const ButtonProp     button_prop;
        static const LayoutProp     layout_prop;

        explicit LayoutManager() = delete;

        static const EnvProp& getEnvProp();
        static void           setup(const QApplication& app);
        static bool           isSetUp();
};

#endif // SHARED_LAYOUT_H
