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

#include <array>
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
                inline static std::array<keybindings, 4> primary_button_keys;

        public:
                static const keybindings& getQuitKeys();
                static const std::array<keybindings, 4>& getPrimaryButtonKeys();
        };
};