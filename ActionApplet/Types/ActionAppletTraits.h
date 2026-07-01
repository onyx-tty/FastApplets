// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"

#include <variant>
#include <QStringView>

template<>
struct AppletTraits<applet::type::action_applet> final {
        using TPrimaryButtonType           = std::monostate;
        constexpr static QStringView title = u"ActionApplet";
};
