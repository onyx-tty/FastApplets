// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/ConfigFile/Properties/Layout.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButton.h"
#include "Core/Config/ConfigFile/Properties/Window.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"
#include "Defaults.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

template<applet::type TApplet>
config::schema::Config config::makeDefaultConfig() {
        using Window             = config::schema::properties::Window;
        using PrimaryButton      = config::schema::properties::PrimaryButton;
        using Layout             = config::schema::properties::Layout;
        using TPrimaryButtonType = AppletTraits<TApplet>::TPrimaryButtonType;

        constexpr QSize size   = {960, 220};
        QString         title  = AppletTraits<TApplet>::title.toString();
        auto            window = Window(size, std::move(title));

        constexpr bool          double_key_press = true;
        constexpr Qt::Alignment text_alignment   = {Qt::AlignHCenter, Qt::AlignTop};
        constexpr Qt::Alignment icon_alignment   = {Qt::AlignHCenter, Qt::AlignVCenter};
        constexpr QSize         icon_size        = {64, 64};
        constexpr QSizePolicy   policy           = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        auto button = PrimaryButton(double_key_press, text_alignment, icon_alignment, icon_size,
                                    policy);

        auto layout = Layout();

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

        layout = Layout(std::move(params));

        return Config(window, button, layout);
}

template<applet::type TApplet>
config::schema::Keys config::makeDefaultKeys() {
        keybindings quit = {Qt::Key_Escape, Qt::Key_Q};

        std::vector<keybindings> primary_buttons = makeKeyRange(Qt::Key_1, Qt::Key_9);

        return Keys(std::move(quit), std::move(primary_buttons));
}
