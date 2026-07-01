// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigFactory.h"
#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

template<applet::type TApplet>
Config ConfigFactory<TApplet>::createDefaultConfig() {
        using TPrimaryButtonType = AppletTraits<TApplet>::TPrimaryButtonType;

        constexpr QSize size   = {960, 220};
        QString         title  = AppletTraits<TApplet>::title.toString();
        auto            window = WindowProperties(size, std::move(title));

        constexpr bool          double_key_press = true;
        constexpr Qt::Alignment text_alignment   = {Qt::AlignHCenter, Qt::AlignTop};
        constexpr Qt::Alignment icon_alignment   = {Qt::AlignHCenter, Qt::AlignVCenter};
        constexpr QSize         icon_size        = {64, 64};
        constexpr QSizePolicy   policy           = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        auto button = PrimaryButtonProperties(double_key_press, text_alignment, icon_alignment,
                                              icon_size, policy);

        auto layout = LayoutProperties();

        constexpr auto param = [](TPrimaryButtonType type) -> PrimaryButtonParams {
                return {.type    = type,
                        .text    = textFor(type),
                        .command = commandFor(type),
                        .icon    = iconFor(type)};
        };

        std::vector<PrimaryButtonParams> params = {};
        if constexpr (TApplet == applet::type::power_applet) {
                using enum power_button_type;

                params = {param(shutdown), param(reboot), param(suspend), param(hibernate)};
        }

        layout = LayoutProperties(std::move(params));

        return Config(window, button, layout);
}

template<applet::type TApplet>
Keys ConfigFactory<TApplet>::createDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_buttons = makeKeyRange(Qt::Key_1, Qt::Key_9);

        return Keys(std::move(quit), std::move(primary_buttons));
}
