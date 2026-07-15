// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletType.h"

namespace applet {

// Stores generic types required for use in Core classes and functions.
//
// To work, each applet must include its own Traits template specializations
// before including the classes requiring them.
template<applet::type T>
struct Traits;

} // namespace applet
