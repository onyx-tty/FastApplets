// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "EnvironmentProperties.h"

EnvironmentProperties::EnvironmentProperties(bool dbus_mode) : dbus_mode(dbus_mode) {}

const bool& EnvironmentProperties::getDBusMode() const {
        return dbus_mode;
}
