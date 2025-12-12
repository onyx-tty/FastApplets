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

// TODO Rework Action, get key or button through a getter
//	and automatically apply updatePowerButton to each use of
//	getButton(), maybe even create a special method that
//	expects the key to be found
struct KeyAction {
        int          key;
        PowerButton* button;

        KeyAction();
        KeyAction(int key, PowerButton* button);

        void reset();
        // Find out if key is a part of any keybindings and adjust
        void updatePowerButton(const std::vector<PowerButton*>& buttons);
};
