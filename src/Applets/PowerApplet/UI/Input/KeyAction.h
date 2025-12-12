/* FastApplets

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

#pragma once

#include "Applets/PowerApplet/UI/Widgets/PowerButton.h"

#include <qnamespace.h>

#include <vector>

class PowerCentralWidget;

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
        KeyAction(int key, PowerButton* button);

        // Find out if key is a part of any keybindings and adjust
        void updatePowerButton(const std::vector<PowerButton*>& buttons);
        int                getKey() const;
        const PowerButton* getButton() const;
        void               reset();
};
