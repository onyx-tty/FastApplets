// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionAppletKeys.h"
#include "Core/Config/KeysFile/Global/GlobalKeys.h"
#include "Core/Config/KeysFile/Mapper/KeysMapper.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <utility>
#include <vector>
#include <Qt>

ActionAppletKeys::ActionAppletKeys(keybindings quit, std::vector<keybindings> primary_button) :
        GlobalKeys(std::move(quit)), primary_button(std::move(primary_button)) {}

const std::vector<keybindings>& ActionAppletKeys::getPrimaryButton() const {
        return primary_button;
}
