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
#include "Config/ConfigMapper.h"
#include "Config/ConfigParser.h"

using std::array;

Keys::GlobalKeys::GlobalKeys(keybindings quit_keys) : quit_keys(quit_keys) {}

const keybindings& Keys::GlobalKeys::getQuitKeys() const {
        return quit_keys;
}

Keys::PowerAppletKeys::PowerAppletKeys(keybindings           quit_keys,
                                       array<keybindings, 4> primary_button_keys) :
        quit_keys(quit_keys), primary_button_keys(primary_button_keys) {}

const keybindings& Keys::PowerAppletKeys::getQuitKeys() const {
        return quit_keys;
}

const array<keybindings, 4>& Keys::PowerAppletKeys::getPrimaryButtonKeys() const {
        return primary_button_keys;
}

Keys::Keys(Keys::GlobalKeys global_keys, Keys::PowerAppletKeys power_applet_keys) :
        global_keys(std::move(global_keys)), power_applet_keys(std::move(power_applet_keys)) {}

Keys& Keys::getKeys() {
        static Keys keys{};
        static bool parsed = false;

        if (!parsed) {
                ConfigMapper::mapToKeys(ConfigParser::createKeys(), keys);
                parsed = true;
        }

        return keys;
}

const Keys::GlobalKeys& Keys::getGlobalKeys() const {
        return global_keys;
}
const Keys::PowerAppletKeys& Keys::getPowerAppletKeys() const {
        return power_applet_keys;
}
