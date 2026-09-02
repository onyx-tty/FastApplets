// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Helpers.h"
#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Shell/Types/ShellContext.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/WindowParams.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <utility>
#include <vector>

constexpr ShellContext config::details::makeShellContext() {
        constexpr bool terminate_on_command_exit = true;

        return ShellContext(terminate_on_command_exit);
}

template<applet::Type TApplet>
WindowParams config::details::makeWindowParams() {
        constexpr QSize size  = {960, 220};
        QString         title = applet::Traits<TApplet>::title.toString();

        return WindowParams(size, std::move(title));
}

template<applet::Type TApplet>
PrimaryButtonParams config::details::makePrimaryButtonParams() {
        using namespace config::details;

        auto           per_button = makePerPrimaryButtonParams<TApplet>();
        constexpr auto style      = makePrimaryButtonStyle();
        constexpr auto behavior   = makePrimaryButtonBehavior();

        return PrimaryButtonParams(std::move(per_button), style, behavior);
}

template<applet::Type TApplet>
std::vector<PerPrimaryButtonParams> config::details::makePerPrimaryButtonParams() {
        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        auto param = [](TPrimaryButtonType type) -> PerPrimaryButtonParams {
                return {.type    = type,
                        .text    = textFor(type),
                        .command = commandFor(type),
                        .icon    = iconFor(type)};
        };

        std::vector<PerPrimaryButtonParams> per_params = {};
        if constexpr (TApplet == applet::Type::Power) {
                using enum PowerButtonType;

                per_params = {param(Shutdown), param(Reboot), param(Suspend), param(Hibernate),
                        param(LogOut)};
        } else if constexpr (TApplet == applet::Type::Action) {
                per_params = {
                        {.text           = "Display greeting notification",
                         .command = "notify-send 'FastApplets' 'Hello!'"                 },
                        {         .text           = "Display current date",
                         .command = "notify-send 'Current date:' \"$(date +'%F %H:%M')\""}
                };
        } else if constexpr (TApplet == applet::Type::Player) {
                using enum PlayerButtonType;

                per_params = {param(PlayPause), param(Next), param(Previous), param(VolumeUp),
                        param(VolumeDown)};
        }

        return per_params;
}
