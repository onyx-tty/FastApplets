// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// Identifies a DBus target; all three fields are required by DBus
struct DBusTarget {
        const char* name;      // e.g. "org.freedesktop.Notifications"
        const char* path;      // e.g. "/org/freedesktop/Notifications"
        const char* interface; // e.g. "org.freedesktop.Notifications"
};
