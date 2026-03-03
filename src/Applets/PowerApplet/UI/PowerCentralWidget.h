/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

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

#pragma once

#include "Config/Keys.h"
#include "Widgets/PowerButton.h"

#include <array>
#include <qnamespace.h>
#include <vector>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QWidget>

enum class power_button { none = 0, shutdown = 1, reboot = 2, suspend = 3, hibernate = 4 };

class PowerCentralWidget final : public QWidget {
        Q_OBJECT

private:
        const keybindings& getKeysFromPowerButton(const PowerButton* power_button);
        PowerButton*       getPowerButtonFromKeys(const keybindings& keys);
        PowerButton*       getPowerButtonFromKey(int key);
        power_button       getSelectedPowerButtonFromPowerButton(const PowerButton* power_button);
        PowerButton*       getPowerButtonFromSelectedPowerButton(power_button button);
        const keybindings& getKeysFromSelectedPowerButton(power_button button);
        power_button       getSelectedPowerButtonFromKeys(const keybindings& keys);
        power_button       getSelectedPowerButtonFromKey(int key);

        std::vector<PowerButton*> createButtonList(QBoxLayout* main_layout);

        QBoxLayout*                     main_layout;
        const std::vector<PowerButton*> button_list;
        power_button                    selected_power_button;

public:
        explicit PowerCentralWidget(QWidget* parent);
        const QBoxLayout*                getMainLayout() const;
        const std::vector<PowerButton*>& getButtonList() const;
        void                             keyPressEvent(QKeyEvent* event) override;
        void                             lastKeyUpdateEvent(QKeyEvent* event);
        void                             keyPressEvent(QKeyEvent* event, PowerButton* button);
        void                             selectButton(QKeyEvent* event, PowerButton* button,
                                                      std::array<int, 4>& acceptable_keys);
        void                             clickButton(QKeyEvent* event);
};
