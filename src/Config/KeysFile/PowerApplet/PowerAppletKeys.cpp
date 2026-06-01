// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletKeys.h"
#include "Config/KeysFile/Global/GlobalKeys.h"
#include "Config/KeysFile/Mapper/KeysMapper.h"
#include "Config/KeysFile/Types/Keybindings.h"

#include <utility>
#include <vector>
#include <Qt>

PowerAppletKeys::PowerAppletKeys(keybindings quit, std::vector<keybindings> primary_button) :
        GlobalKeys(std::move(quit)), primary_button(std::move(primary_button)) {}

const std::vector<keybindings>& PowerAppletKeys::getPrimaryButton() const {
        return primary_button;
}
