/* Pretty Applets

   Copyright (C) 2025 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <QDBusMessage>
#include <QString>

// org.freedesktop.login1 — The D-Bus interface of systemd-logind
struct DBusTarget {
        static constexpr const char* name      = "org.freedesktop.login1";
        static constexpr const char* path      = "/org/freedesktop/login1";
        static constexpr const char* interface = "org.freedesktop.login1.Manager";
};

class PowerActionManager final {
public:
        static QDBusMessage sendPowerAction(const QString& method);

private:
        explicit PowerActionManager() = delete;

        static QDBusMessage handleResponse(QDBusMessage response);
};
