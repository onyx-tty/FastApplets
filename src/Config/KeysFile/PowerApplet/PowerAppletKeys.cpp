// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletKeys.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/FileLocator/FileLocator.h"
#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Mapper/KeysMapper.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/TomlParser/TomlParser.h"
#include "Config/Types/ConfigFiles.h"

#include <utility>
#include <vector>
#include <Qt>

PowerAppletKeys::PowerAppletKeys(keybindings quit, std::vector<keybindings> primary_button) :
        GlobalKeys(std::move(quit)), primary_button(std::move(primary_button)) {}

const PowerAppletKeys& PowerAppletKeys::get() {
        // TODO: Config files should not be fetched twice, once for config, once for keys.
        //       Either fetch them individually or cache the result for both.
        static const ConfigFiles power = FileLocator::locateConfigFiles(applet::power_applet.scope);
        static const ConfigFiles global = FileLocator::locateConfigFiles(applet::global.scope);

        static const auto keys =
                KeysMapper::mapToPowerAppletKeys<PowerAppletKeys>(TomlParser::parseFile(power.keys),
                                                                  TomlParser::parseFile(
                                                                          global.keys));

        return keys;
}

const PowerAppletKeys& PowerAppletKeys::getDefault() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_button = {
                keybindings{Qt::Key_1}, keybindings{Qt::Key_2}, keybindings{Qt::Key_3},
                keybindings{Qt::Key_4}, keybindings{Qt::Key_5}, keybindings{Qt::Key_6},
                keybindings{Qt::Key_7}, keybindings{Qt::Key_8}, keybindings{Qt::Key_9},
        };

        static PowerAppletKeys keys{std::move(quit), std::move(primary_button)};

        return keys;
}

const std::vector<keybindings>& PowerAppletKeys::getPrimaryButton() const {
        return primary_button;
}
