/* Pretty Applets

   Copyright (C) 2024 Łukasz Wrodarczyk

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

#ifndef POWER_ACTION_MANAGER_H
#define POWER_ACTION_MANAGER_H

#include <QtDBus/QtDBus> // TODO Don't include directly
#include <QString>

// org.freedesktop.login1 — The D-Bus interface of systemd-logind
namespace target {
extern const QString name;
extern const QString path;
extern const QString interface;
}

class PowerActionManager final { // Singleton design

public:
        PowerActionManager(const PowerActionManager&) = delete;
        PowerActionManager& operator=(const PowerActionManager&) = delete;
        static PowerActionManager& getInstance();

        QDBusMessage sendPowerAction(const QString& method) const;

private:
        QDBusConnection connection;
        QDBusInterface proxy;

        explicit PowerActionManager(QObject* parent = nullptr);

        QDBusMessage responseHandler(QDBusMessage response) const;

};

#endif // POWER_ACTION_MANAGER_H
