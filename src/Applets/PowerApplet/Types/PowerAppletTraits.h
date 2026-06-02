// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/PowerApplet/UI/Widgets/PowerButton.h"
#include "Applets/Types/AppletTraits.h"
#include "Applets/Types/AppletType.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <string_view>

template<>
struct AppletTraits<applet::type::power_applet> final {
        using TConfig                        = PowerAppletConfig;
        using TKeys                          = PowerAppletKeys;
        using TPrimaryButton                 = PowerButton;
        using TPrimaryButtonParams           = PowerButtonParams;
        using TPrimaryButtonType             = power_button_type;
        inline static std::string_view title = "PowerApplet";
};
