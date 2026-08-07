// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <QSize>
#include <QSizePolicy>
#include <Qt>

#include <vector>

namespace config::details {

template<applet::Type TApplet>
WindowParams makeWindowParams();

template<applet::Type TApplet>
PrimaryButtonParams makePrimaryButtonParams();

template<applet::Type TApplet>
std::vector<PerPrimaryButtonParams> makePerPrimaryButtonParams();

constexpr PrimaryButtonStyle makePrimaryButtonStyle() {
        constexpr Qt::Alignment text_alignment = {Qt::AlignHCenter, Qt::AlignTop};
        constexpr Qt::Alignment icon_alignment = {Qt::AlignHCenter, Qt::AlignVCenter};
        constexpr QSize         icon_size      = {64, 64};
        constexpr QSizePolicy   policy         = {QSizePolicy::Expanding, QSizePolicy::Expanding};
        constexpr auto style = PrimaryButtonStyle(text_alignment, icon_alignment, icon_size, policy);

        return style;
}

constexpr PrimaryButtonBehavior makePrimaryButtonBehavior() {
        constexpr bool double_key_press = true;
        constexpr auto behavior         = PrimaryButtonBehavior(double_key_press);

        return behavior;
}

} // namespace config::details

#include "Helpers.tpp"
