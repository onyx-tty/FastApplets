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

#include "DBusRequester.h"
#include "CppUtils/Log/QtLog.h"
#include "DBusTarget.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QList>
#include <QVariant>
#include <QVariantMap>

QDBusPendingReply<QVariantMap> DBusRequester::pending_reply;

void DBusRequester::call(const DBusTarget& target, const QString& method) {
        static const auto connection = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                                     target.name);
        static const auto proxy      = QDBusInterface(target.name, target.path, target.interface,
                                                      connection, nullptr);
        if (!proxy.isValid()) { QFATAL("D-Bus proxy is invalid!"); }

        auto call = QDBusMessage::createMethodCall(target.name, target.path, target.interface,
                                                   method);

        QList<QVariant> arguments;
        arguments << QVariant::fromValue(true);
        call.setArguments(arguments);

        pending_reply = connection.asyncCall(call);
}

void DBusRequester::waitForFinished() {
        pending_reply.waitForFinished();
}

const QDBusPendingReply<QVariantMap>& DBusRequester::getReply() {
        waitForFinished();
        return pending_reply;
}
