// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletKeys.h"
#include "Applets/Types/AppletTypes.h"
#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Mapper/KeysMapper.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/TomlParser/TomlParser.h"
#include "Config/Types/ConfigType.h"

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
                KeysMapper::mapToPowerAppletKeys(TomlParser::parseFile(applet::type::power_applet,
                                                                       config::type::keys),
                                                 TomlParser::parseFile(applet::type::global,
                                                                       config::type::keys),
                                                 keys);
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
