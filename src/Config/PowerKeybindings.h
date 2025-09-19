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

#ifndef POWER_KEYBINDINGS_H
#define POWER_KEYBINDINGS_H

#include "SharedKeybindings.h"

#include <unordered_set>

struct PowerKeybindingManager : public KeybindingManager {
        std::unordered_set<int> shutdown;
        std::unordered_set<int> reboot;
        std::unordered_set<int> suspend;
        std::unordered_set<int> hibernate;

        explicit PowerKeybindingManager();
};

#endif // POWER_KEYBINDINGS_H
