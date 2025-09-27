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

// TODO Check for validity of power action
QDBusMessage PowerActionManager::sendPowerAction(const QString& method) {
        const auto connection = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                              DBusTarget::name);
        const auto proxy = QDBusInterface(DBusTarget::name, DBusTarget::path, DBusTarget::interface,
                                          connection, nullptr);
        if (!proxy.isValid()) { qFatal("D-Bus proxy is invalid!"); }

        auto            call = QDBusMessage::createMethodCall(DBusTarget::name, DBusTarget::path,
                                                              DBusTarget::interface, method);
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
