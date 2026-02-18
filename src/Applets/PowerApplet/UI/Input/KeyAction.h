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

#include "Applets/PowerApplet/UI/Widgets/PowerButton.h"

#include <qnamespace.h>
#include <vector>

class PowerCentralWidget;

PowerButton* findPowerButton(int key_to_find, const std::vector<PowerButton*>& buttons);

class KeyAction {
private:
        // TODO Remove friendship for safety, only required for animateClick and setFocus, needs a rework!
        friend PowerCentralWidget;

        int          key;
        PowerButton* button;

        PowerButton* debugGetButtonNonConst();

public:
        // Create empty
        KeyAction();
        // TODO Remove, only exists to temporarily permit decoupled key and button
        KeyAction(int key, PowerButton* button);
        // Fully initialize and automatically find the right button
        // TODO Eliminate passing of buttons by accessing PowerCentralWidget's button_list
        KeyAction(const std::vector<PowerButton*>& buttons, int key);
        KeyAction(const KeyAction& other);
        KeyAction(KeyAction&& other);
        KeyAction& operator=(const KeyAction& other);
        KeyAction& operator=(KeyAction&& other);

        int                getKey() const;
        const PowerButton* getButton() const;
        void               reset();
};
