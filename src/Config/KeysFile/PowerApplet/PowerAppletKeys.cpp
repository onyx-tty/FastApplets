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
#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Mapper/KeysMapper.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/TOML/TomlParser.h"

#include <utility>
#include <vector>
#include <Qt>

PowerAppletKeys::PowerAppletKeys(keybindings              quit_keys,
                                 std::vector<keybindings> primary_button_keys) :
        GlobalKeys(std::move(quit_keys)), primary_button_keys(std::move(primary_button_keys)) {}

PowerAppletKeys& PowerAppletKeys::get() {
        static PowerAppletKeys keys{};
        static bool            parsed = false;

        if (!parsed) {
                KeysMapper::mapToPowerAppletKeys(TomlParser::parsePowerAppletKeys(),
                                                 TomlParser::parseGlobalKeys(), keys);
                parsed = true;
        }

        return keys;
}

const PowerAppletKeys& PowerAppletKeys::getDefault() {
        keybindings quit_keys = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_button_keys = {
                keybindings{Qt::Key_1}, keybindings{Qt::Key_2}, keybindings{Qt::Key_3},
                keybindings{Qt::Key_4}, keybindings{Qt::Key_5}, keybindings{Qt::Key_6},
                keybindings{Qt::Key_7}, keybindings{Qt::Key_8}, keybindings{Qt::Key_9},
        };

        static PowerAppletKeys default_keys{std::move(quit_keys), std::move(primary_button_keys)};

        return default_keys;
}

const std::vector<keybindings>& PowerAppletKeys::getPrimaryButtonKeys() const {
        return primary_button_keys;
}
