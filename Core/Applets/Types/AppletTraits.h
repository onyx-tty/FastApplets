// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AppletType.h"

// Stores generic types required for use in Core classes and functions.
// They're used to avoid including each applet's classes in Core classes, which
// would prevent Core from being isolated from the applets using it.
//
// To work, each applet must include its own AppletTraits template specializations
// before including the classes requiring them.
template<applet::type T>
struct AppletTraits;
