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

#include "PowerActionManager.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QDebug>
#include <QList>
#include <QVariant>
#include <QVariantMap>

// org.freedesktop.login1 — The D-Bus interface of systemd-logind
namespace dbus_target {
static const char* name      = "org.freedesktop.login1";
static const char* path      = "/org/freedesktop/login1";
static const char* interface = "org.freedesktop.login1.Manager";
} // namespace dbus_target

// TODO Check for validity of power action
QDBusMessage PowerActionManager::sendPowerAction(const QString& method) {
        const auto connection = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                              dbus_target::name);
        const auto proxy      = QDBusInterface(dbus_target::name, dbus_target::path,
                                               dbus_target::interface, connection, nullptr);
        if (!proxy.isValid()) { qFatal("D-Bus proxy is invalid!"); }

        auto            call = QDBusMessage::createMethodCall(dbus_target::name, dbus_target::path,
                                                              dbus_target::interface, method);
        QList<QVariant> arguments;
        arguments << QVariant::fromValue(true);
        call.setArguments(arguments);

        QDBusPendingReply<QVariantMap> response = connection.asyncCall(call);
        response.waitForFinished();
        return handleResponse(std::move(response.reply()));
}

QDBusMessage PowerActionManager::handleResponse(QDBusMessage response) {
        if (response.type() == QDBusMessage::ErrorMessage) {
                qCritical() << "Error sending action. Response: " << response.errorMessage();
        } else {
                qInfo() << response;
        }

        return std::move(response);
}
