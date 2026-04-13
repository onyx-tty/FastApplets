/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

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

#include "PowerActionManager.h"
#include "Log/Log.h"
#include "DBus/DBusRequester.h"
#include "DBus/DBusTarget.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QList>
#include <QVariant>
#include <QVariantMap>

void PowerActionManager::printReply(const QDBusPendingReply<QVariantMap>& reply) {
        if (reply.reply().type() == QDBusMessage::ErrorMessage) {
                QCRITICAL() << "Error sending action! Response:" << reply.error().name() << "-"
                            << reply.error().message();
        } else {
                QINFO() << "Success sending power action!";
        }
}

// TODO Check for validity of power action
// org.freedesktop.login1 — The D-Bus interface of systemd-logind
void PowerActionManager::sendPowerAction(const QString& method) {
        constexpr const char* name           = "org.freedesktop.login1";
        constexpr const char* path           = "/org/freedesktop/login1";
        constexpr const char* interface      = "org.freedesktop.login1.Manager";
        static constexpr auto systemd_logind = DBusTarget{name, path, interface};

        QDEBUG() << "Sending signal" << method << "to" << interface;
        DBusRequester::call(systemd_logind, method);
        printReply(DBusRequester::getReply());
}
