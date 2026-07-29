// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/Defaults/Helpers/Helpers.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Defaults.h"

#include <utility>
#include <vector>
#include <Qt>

template<applet::type TApplet>
config::schema::Config config::makeDefaultConfig() {
        using namespace config::details;

        auto window = makeWindowParams<TApplet>();
        auto params = makePrimaryButtonParams<TApplet>();

        return Config(std::move(window), std::move(params));
}

template<applet::type TApplet>
config::schema::Keys config::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_buttons = makeKeyRange(Qt::Key_1, Qt::Key_9);

        return Keys(std::move(quit), std::move(primary_buttons));
}
