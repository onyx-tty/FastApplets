// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GlobalKeys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <utility>

GlobalKeys::GlobalKeys(keybindings quit) : quit(std::move(quit)) {}

const keybindings& GlobalKeys::getQuit() const {
        return quit;
}
