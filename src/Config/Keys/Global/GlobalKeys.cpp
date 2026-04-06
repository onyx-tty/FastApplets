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

#include "GlobalKeys.h"
#include "Config/Keys/Keybindings/Keybindings.h"

#include <utility>

GlobalKeys::GlobalKeys(keybindings quit_keys) : quit_keys(std::move(quit_keys)) {}

const keybindings& GlobalKeys::getQuitKeys() const {
        return quit_keys;
}
