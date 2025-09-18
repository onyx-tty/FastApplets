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

#include "PowerLayout.h"
#include "../Applets/PowerApplet/Widgets/PowerButton.h"

#include <QApplication>
#include <QSizePolicy>
#include <QString>

#include <array>

using global = LayoutManager;

/* These are settings exclusive to the PowerApplet. Modify below to adjust application style. */
// To overwrite defaults, replace parts preceded by the global namespace.

std::array<QIcon, 4> setButtonIcons() {
        Q_INIT_RESOURCE(Icons);
        std::array<QIcon, 4> button_icons{QIcon(":/Icons/shutdown.svg"),
                                          QIcon(":/Icons/reboot.svg"),
                                          QIcon(":/Icons/suspend.svg"),
                                          QIcon(":/Icons/hibernate.svg")};
        return button_icons;
}

PowerMainWindowProp::PowerMainWindowProp() :
        size(global::main_window_prop.size), title(global::main_window_prop.title) {};

PowerStyleProp::PowerStyleProp() :
        selected(global::style_prop.selected), unselected(global::style_prop.unselected) {};

// TODO Text size
PowerButtonProp::PowerButtonProp() :
        text_alignment(global::button_prop.text_alignment),
        icon_size(global::button_prop.icon_size),
        icon_alignment(global::button_prop.icon_alignment) {};

PowerLayoutProp::PowerLayoutProp() :
        button_policy(global::layout_prop.button_policy), button_icons(setButtonIcons()),
        button_text({"Shutdown", "Reboot", "Suspend", "Hibernate"}) {};

PowerLayoutManager::PowerLayoutManager() {};

std::array<PowerButton*, 4>& PowerLayoutProp::buttonListSingleton(QWidget*     parent,
                                                                  QHBoxLayout* layout,
                                                                  bool&&       is_instantiated) {
        if (!parent || !layout) {
                qFatal() << "Invalid parent and/or layout in" << __func__ << "!\n";
                QApplication::quit();
        }
        if (!is_instantiated) {
                qCritical() << "Singleton not initialized properly in" << __func__ << "!\n";
                QApplication::quit();
        }

        static std::array<PowerButton*, 4> button_list{
                new PowerButton(parent, layout, button_icons[0], button_text[0], "PowerOff"),
                new PowerButton(parent, layout, button_icons[1], button_text[1], "Reboot"),
                new PowerButton(parent, layout, button_icons[2], button_text[2], "Suspend"),
                new PowerButton(parent, layout, button_icons[3], button_text[3], "Hibernate")};

        return button_list;
}

PowerMainWindowProp PowerLayoutManager::main_window_prop;
PowerStyleProp      PowerLayoutManager::style_prop;
PowerButtonProp     PowerLayoutManager::button_prop;
PowerLayoutProp     PowerLayoutManager::layout_prop;
