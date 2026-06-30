// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"

#include <string_view>
#include <variant>

template<>
struct AppletTraits<applet::type::action_applet> final {
        using TPrimaryButtonType             = std::monostate;
        inline static std::string_view title = "ActionApplet";
};
