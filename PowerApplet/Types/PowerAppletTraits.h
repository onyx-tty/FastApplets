// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/UI/Types/ButtonType.h"

#include <string_view>

template<>
struct AppletTraits<applet::type::power_applet> final {
        using TPrimaryButtonType             = power_button_type;
        inline static std::string_view title = "PowerApplet";
};
