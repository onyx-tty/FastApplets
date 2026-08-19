// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Defaults.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/Defaults/Helpers/Helpers.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/Config/Types/Keybindings.h"

#include <Qt>
#include <utility>
#include <vector>

template<applet::Type TApplet>
config::schema::Config config::makeDefaultConfig() {
        using namespace config::details;

        auto window = makeWindowParams<TApplet>();
        auto params = makePrimaryButtonParams<TApplet>();

        return Config(std::move(window), std::move(params));
}

template<applet::Type TApplet>
config::schema::Keys config::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_buttons = makeKeyRange(Qt::Key_1, Qt::Key_9);

        return Keys(std::move(quit), std::move(primary_buttons));
}
