// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"

#include <variant>
#include <QStringView>

namespace applet {

template<>
struct Traits<applet::type::action> final {
        using TPrimaryButtonType           = std::monostate;
        constexpr static QStringView title = u"ActionApplet";
};

} // namespace applet
