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

#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Keybindings/Keybindings.h"

#include <array>

class KeysMapper;

class PowerAppletKeys final : public GlobalKeys {
private:
        friend class KeysMapper;

        std::array<keybindings, 4> primary_button_keys;

public:
        explicit PowerAppletKeys(keybindings                quit_keys           = {},
                                 std::array<keybindings, 4> primary_button_keys = {});
        // TODO Make this const to avoid overwrites
        static PowerAppletKeys&           get();
        static const PowerAppletKeys&     getDefault();
        const std::array<keybindings, 4>& getPrimaryButtonKeys() const;
};
