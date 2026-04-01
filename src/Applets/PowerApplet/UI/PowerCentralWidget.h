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

#include "Config/Keys/Keybindings/Keybindings.h"
#include "Config/Keys/PowerApplet/PowerAppletKeys.h"
#include "UI/Enums/ButtonIDs.h"
#include "Widgets/PowerButton.h"

#include <qnamespace.h>
#include <vector>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QWidget>

class PowerCentralWidget final : public QWidget {
        Q_OBJECT

private:
        const keybindings&        getKeysFromPowerButton(const PowerButton* power_button);
        PowerButton*              getPowerButtonFromKeys(const keybindings& keys);
        PowerButton*              getPowerButtonFromKey(int key);
        PowerButton*              getPowerButtonFromPowerButtonID(power_button_id button);
        const keybindings&        getKeysFromPowerButtonID(power_button_id button);
        power_button_id           getPowerButtonIDFromKeys(const keybindings& keys);
        power_button_id           getPowerButtonIDFromKey(int key);
        std::vector<PowerButton*> createButtonList(QBoxLayout* main_layout);

        QBoxLayout*                     main_layout;
        const std::vector<PowerButton*> button_list;

public:
        explicit PowerCentralWidget(QWidget* parent);
        const QBoxLayout*                getMainLayout() const;
        const std::vector<PowerButton*>& getButtonList() const;
        void                             keyPressEvent(QKeyEvent* event) override;
};
