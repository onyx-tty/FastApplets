// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GlobalKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"

#include <utility>

GlobalKeys::GlobalKeys(keybindings quit_keys) : quit_keys(std::move(quit_keys)) {}

const keybindings& GlobalKeys::getQuitKeys() const {
        return quit_keys;
}
