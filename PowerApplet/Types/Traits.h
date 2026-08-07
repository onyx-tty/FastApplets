// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/UI/Types/ButtonType.h"

#include <QStringView>

namespace applet {

template<>
struct Traits<applet::Type::Power> final {
        using TPrimaryButtonType           = PowerButtonType;
        constexpr static QStringView title = u"PowerApplet";
};

} // namespace applet
