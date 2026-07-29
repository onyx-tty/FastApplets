// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/WindowParams.h"
#include "Helpers.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

template<applet::type TApplet>
WindowParams config::details::makeWindowParams() {
        constexpr QSize size  = {960, 220};
        QString         title = applet::Traits<TApplet>::title.toString();

        return WindowParams(size, std::move(title));
}

template<applet::type TApplet>
std::vector<PerPrimaryButtonParams> config::details::makePerPrimaryButtonParams() {
        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        auto param = [](TPrimaryButtonType type) -> PerPrimaryButtonParams {
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

        return per_params;
}
