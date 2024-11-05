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

#include "ButtonList.h"
#include "../DBus/PowerActionManager.h"
#include "../../../Config/PowerLayout.h"

#include <QDebug>

PowerButton* power_button::shutdown = nullptr;
PowerButton* power_button::reboot = nullptr;
PowerButton* power_button::suspend = nullptr;
PowerButton* power_button::hibernate = nullptr;

void power_button::initAll(QWidget* parent, QVBoxLayout* layout) {
        power_button::shutdown = new PowerButton(parent, layout, icon::shutdown, text::shutdown);
        PowerButton::connect(power_button::shutdown, &PowerButton::clicked, []() {
                qInfo() << "Initiating shutdown...\n";
                PowerActionManager::getInstance().shutdown();
        });

        power_button::reboot = new PowerButton(parent, layout, icon::reboot, text::reboot);
        PowerButton::connect(power_button::reboot, &PowerButton::clicked, []() {
                qInfo() << "Initiating reboot...\n";
                PowerActionManager::getInstance().reboot();
        });

        power_button::suspend = new PowerButton(parent, layout, icon::suspend, text::suspend);
        PowerButton::connect(power_button::suspend, &PowerButton::clicked, []() {
                qInfo() << "Initiating suspension...\n";
                PowerActionManager::getInstance().suspend();
        });

        power_button::hibernate = new PowerButton(parent, layout, icon::hibernate, text::hibernate);
        PowerButton::connect(power_button::hibernate, &PowerButton::clicked, []() {
                qInfo() << "Initiating hibernation...\n";
                PowerActionManager::getInstance().hibernate();
        });
}
