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

#include <array>
#include <unordered_set>

class ConfigMapper;

using keybindings = std::unordered_set<int>;

class Keys final {
public:
        class GlobalKeys final {
        private:
                friend class ConfigMapper;

                keybindings quit_keys;

        public:
                GlobalKeys(keybindings quit_keys = {});
                const keybindings& getQuitKeys() const;
        };

        class PowerAppletKeys final {
        private:
                friend class ConfigMapper;

                keybindings                quit_keys;
                std::array<keybindings, 4> primary_button_keys;

        public:
                PowerAppletKeys(keybindings                quit_keys           = {},
                                std::array<keybindings, 4> primary_button_keys = {});
                const keybindings&                getQuitKeys() const;
                const std::array<keybindings, 4>& getPrimaryButtonKeys() const;
        };

        Keys(Keys::GlobalKeys global_keys = {}, Keys::PowerAppletKeys power_applet_keys = {});
        // TODO Make this const to avoid overwrites
        static Keys&           getKeys();
        const GlobalKeys&      getGlobalKeys() const;
        const PowerAppletKeys& getPowerAppletKeys() const;

private:
        friend class ConfigMapper;

        GlobalKeys      global_keys;
        PowerAppletKeys power_applet_keys;
};
