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

#include "PowerAppletKeys.h"

#include <array>

PowerAppletKeys::PowerAppletKeys(keybindings                quit_keys,
                                 std::array<keybindings, 4> primary_button_keys) :
        quit_keys(quit_keys), primary_button_keys(primary_button_keys) {}

const keybindings& PowerAppletKeys::getQuitKeys() const {
        return quit_keys;
}

const std::array<keybindings, 4>& PowerAppletKeys::getPrimaryButtonKeys() const {
        return primary_button_keys;
}
