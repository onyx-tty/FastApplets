// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <utility>
#include <vector>

config::schema::Keys::Keys(keybindings quit, std::vector<keybindings> primary_buttons) :
        quit(std::move(quit)), primary_buttons(std::move(primary_buttons)) {}

const keybindings& config::schema::Keys::getQuit() const {
        return quit;
}

const std::vector<keybindings>& config::schema::Keys::getPrimaryButtons() const {
        return primary_buttons;
}
