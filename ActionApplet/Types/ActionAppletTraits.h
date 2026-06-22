// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ActionApplet/UI/Widgets/ActionButton.h"
#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/ActionApplet/ActionAppletConfig.h"
#include "Core/Config/KeysFile/ActionApplet/ActionAppletKeys.h"
#include "Core/UI/Widgets/ActionButtonParams.h"

#include <string_view>

template<>
struct AppletTraits<applet::type::action_applet> final {
        using TConfig                        = ActionAppletConfig;
        using TKeys                          = ActionAppletKeys;
        using TPrimaryButton                 = ActionButton;
        using TPrimaryButtonParams           = ActionButtonParams;
        inline static std::string_view title = "ActionApplet";
};
