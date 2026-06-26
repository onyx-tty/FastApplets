// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/ActionApplet/ActionAppletConfig.h"
#include "Core/Config/KeysFile/ActionApplet/ActionAppletKeys.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <string_view>

template<>
struct AppletTraits<applet::type::action_applet> final {
        using TConfig                        = ActionAppletConfig;
        using TKeys                          = ActionAppletKeys;
        using TPrimaryButtonParams           = PrimaryButtonParams;
        inline static std::string_view title = "ActionApplet";
};
