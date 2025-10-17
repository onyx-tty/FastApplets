/* Pretty Applets

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

#include <unordered_set>

class TomlConfigParser;

using keybindings = std::unordered_set<int>;

class Keys final {
public:
        class GlobalKeys final {
        private:
                friend class TomlConfigParser;
                
                inline static keybindings quit_keys;
        
        public:
                static const keybindings& getQuitKeys();
        };

        class PowerAppletKeys final {
        private:
                friend class TomlConfigParser;
                
                inline static keybindings quit_keys;
                inline static keybindings primary_button1_keys;
                inline static keybindings primary_button2_keys;
                inline static keybindings primary_button3_keys;
                inline static keybindings primary_button4_keys;

        public:
                static const keybindings& getQuitKeys();
                static const keybindings& getPrimaryButton1Keys();
                static const keybindings& getPrimaryButton2Keys();
                static const keybindings& getPrimaryButton3Keys();
                static const keybindings& getPrimaryButton4Keys();
        };
};