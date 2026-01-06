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

#include "Keys.h"

using std::array;

const keybindings& Keys::GlobalKeys::getQuitKeys(){ 
        return Keys::GlobalKeys::quit_keys;
}

const keybindings& Keys::PowerAppletKeys::getQuitKeys() {
        return Keys::PowerAppletKeys::quit_keys;
}

const array<keybindings, 4>& Keys::PowerAppletKeys::getPrimaryButtonKeys() {
        return Keys::PowerAppletKeys::primary_button_keys;
}
