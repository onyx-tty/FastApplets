// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class ConfigMapper;

class EnvironmentProperties final {
private:
        friend class ConfigMapper;

        bool dbus_mode;

public:
        explicit EnvironmentProperties(bool dbus_mode = false);
        const bool& getDBusMode() const;
};
