// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <utility>
#include <vector>

Keys::Keys(keybindings quit, std::vector<keybindings> primary_buttons) :
        quit(std::move(quit)), primary_buttons(std::move(primary_buttons)) {}

const keybindings& Keys::getQuit() const {
        return quit;
}

const std::vector<keybindings>& Keys::getPrimaryButtons() const {
        return primary_buttons;
}
