// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"
#include "Defaults.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

template<applet::type TApplet>
config::schema::Config config::makeDefaultConfig() {
        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        constexpr QSize size   = {960, 220};
        QString         title  = applet::Traits<TApplet>::title.toString();
        auto            window = WindowParams(size, std::move(title));

        constexpr Qt::Alignment text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        constexpr Qt::Alignment icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        constexpr QSize         icon_size      = {64, 64};
        constexpr QSizePolicy   policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        // TODO: constexpr
        auto style = PrimaryButtonStyle(text_alignment, icon_alignment, icon_size, policy);

        constexpr bool double_key_press = true;
        constexpr auto behavior         = PrimaryButtonBehavior(double_key_press);

        constexpr auto param = [](TPrimaryButtonType type) -> PerPrimaryButtonParams {
                return {.type    = type,
                        .text    = textFor(type),
                        .command = commandFor(type),
                        .icon    = iconFor(type)};
        };

        std::vector<PerPrimaryButtonParams> per_params = {};
        if constexpr (TApplet == applet::type::power) {
                using enum power_button_type;

                per_params = {param(shutdown), param(reboot), param(suspend), param(hibernate)};
        } else if constexpr (TApplet == applet::type::action) {
                per_params = {{.text    = "Display greeting notification",
                               .command = "notify-send 'FastApplets' 'Hello!'"},
                              {.text    = "Display current date",
                               .command = "notify-send 'Current date:' \"$(date +'%F %H:%M')\""}};
        } else if constexpr (TApplet == applet::type::player) {
                using enum volume_button_type;

                per_params = {param(play_pause), param(next), param(previous), param(volume_up),
                              param(volume_down)};
        }

        auto params = PrimaryButtonParams(std::move(per_params), std::move(style), behavior);

        return Config(std::move(window), std::move(params));
}

template<applet::type TApplet>
config::schema::Keys config::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_buttons = makeKeyRange(Qt::Key_1, Qt::Key_9);

        return Keys(std::move(quit), std::move(primary_buttons));
}
