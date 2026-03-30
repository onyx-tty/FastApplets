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
#include "Config/TOML/TomlParser.h"
#include "Global/GlobalKeys.h"
#include "Keybindings/Keybindings.h"
#include "KeysMapper.h"
#include "PowerApplet/PowerAppletKeys.h"

#include <qnamespace.h>
#include <utility>

Keys::Keys(GlobalKeys global_keys, PowerAppletKeys power_applet_keys) :
        global_keys(std::move(global_keys)), power_applet_keys(std::move(power_applet_keys)) {}

Keys& Keys::get() {
        static Keys keys{};
        static bool parsed = false;

        if (!parsed) {
                KeysMapper::mapToKeys(TomlParser::createKeys(), keys);
                parsed = true;
        }

        return keys;
}

const Keys& Keys::getDefault() {
        keybindings quit_keys           = {Qt::Key_Escape, Qt::Key_Q};
        GlobalKeys  default_global_keys = {std::move(quit_keys)};

        std::array<keybindings, 4> primary_button_keys       = {keybindings{Qt::Key_1},
                                                                keybindings{Qt::Key_2},
                                                                keybindings{Qt::Key_3},
                                                                keybindings{Qt::Key_4}};
        PowerAppletKeys            default_power_applet_keys = {default_global_keys.getQuitKeys(),
                                                                std::move(primary_button_keys)};

        static Keys default_keys{std::move(default_global_keys),
                                 std::move(default_power_applet_keys)};

        return default_keys;
}

const GlobalKeys& Keys::getGlobalKeys() const {
        return global_keys;
}
const PowerAppletKeys& Keys::getPowerAppletKeys() const {
        return power_applet_keys;
}
