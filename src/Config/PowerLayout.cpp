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

#include "PowerLayout.h"
#include "../Applets/PowerApplet/Widgets/PowerButton.h"

#include <QApplication>
#include <QSizePolicy>
#include <QString>

#include <array>

using global = LayoutManager;

/* These are settings exclusive to the PowerApplet. Modify below to adjust application style. */
/* To overwrite defaults, replace parts preceded by the global namespace. */
/* For modified defaults, refer to DefaultLayout.cpp */

/* Button attributes unique to this layout */
// prevent external linkage via static
static std::array<QIcon, 4> initButtonIcons() {
        Q_INIT_RESOURCE(Icons);
        std::array<QIcon, 4> button_icons{QIcon(":/Icons/shutdown.svg"),
                                          QIcon(":/Icons/reboot.svg"),
                                          QIcon(":/Icons/suspend.svg"),
                                          QIcon(":/Icons/hibernate.svg")};
        return std::move(button_icons);
}

// prevent external linkage via static
static std::array<QString, 4> initButtonText() {
        std::array<QString, 4> button_text = {"Shutdown", "Reboot", "Suspend", "Hibernate"};
        return std::move(button_text);
}

/* PowerLayoutManager */
const PowerMainWindowProp PowerLayoutManager::main_window_prop(global::main_window_prop.size,
                                                               global::main_window_prop.title);
const PowerStyleProp      PowerLayoutManager::style_prop(global::style_prop.button_stylesheet);
const PowerButtonProp     PowerLayoutManager::button_prop(global::button_prop.text_alignment,
                                                          global::button_prop.icon_size,
                                                          global::button_prop.icon_alignment);
PowerLayoutProp           PowerLayoutManager::layout_prop(global::layout_prop.button_policy,
                                                          initButtonIcons(), initButtonText());
/* END CONFIG */

/* PowerMainWindowProp */
PowerMainWindowProp::PowerMainWindowProp(const QSize size, const QString title) :
        MainWindowProp(size, title), size(size), title(title) {};

/* PowerStyleProp */
PowerStyleProp::PowerStyleProp(const QString button_stylesheet) :
        StyleProp(button_stylesheet), button_stylesheet(button_stylesheet) {};

/* PowerButtonProp */
// TODO Text size
PowerButtonProp::PowerButtonProp(const Qt::Alignment text_alignment, const QSize icon_size,
                                 const Qt::Alignment icon_alignment) :
        ButtonProp(text_alignment, icon_size, icon_alignment), text_alignment(text_alignment),
        icon_size(icon_size), icon_alignment(icon_alignment) {};

/* PowerLayoutProp */
PowerLayoutProp::PowerLayoutProp(const QSizePolicy            button_policy,
                                 const std::array<QIcon, 4>   button_icons,
                                 const std::array<QString, 4> button_text) :
        LayoutProp(button_policy), button_policy(button_policy), button_icons(button_icons),
        button_text(button_text) {};

void PowerLayoutProp::initButtonList(QBoxLayout* layout) {
        if (!layout) {
                qFatal() << "Invalid layout in" << __func__ << "!\n";
                QApplication::quit();
        }
        if (button_list) {
                qWarning() << "Button list is already initialized but there was an attempt to"
                           << "initialize it once more. Attempted changes have been discarded.";
        }

        // extracting button icons and text from given instance to reduce verbosity
        static std::array<PowerButton*, 4> isolated_button_list{
                new PowerButton(layout, button_icons[0], button_text[0], "PowerOff"),
                new PowerButton(layout, button_icons[1], button_text[1], "Reboot"),
                new PowerButton(layout, button_icons[2], button_text[2], "Suspend"),
                new PowerButton(layout, button_icons[3], button_text[3], "Hibernate"),
        };

        button_list = &isolated_button_list;

}

const std::array<PowerButton*, 4>* PowerLayoutProp::getButtonList() const {
        if (button_list) return button_list;
        else qFatal("Button list not initialized yet, access denied!");
}

bool PowerLayoutProp::isSetUp() {
        return button_list;
}

const PowerLayoutProp& PowerLayoutManager::getLayoutProp() {
        return layout_prop;
}

void PowerLayoutManager::setup(const QApplication& app, QBoxLayout* layout) {
        LayoutManager::setup(app);
        layout_prop.initButtonList(layout);
}

bool PowerLayoutManager::isSetUp() {
        return LayoutManager::isSetUp() && layout_prop.isSetUp();
}
