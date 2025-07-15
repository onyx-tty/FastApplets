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

/* public */
PowerActionManager& PowerActionManager::getInstance() {
        static PowerActionManager instance;
        return instance;
}

QDBusMessage PowerActionManager::sendPowerAction(const QString& method) const {
        if (!proxy.isValid()) { qFatal("D-Bus proxy is invalid!"); }

        auto            call = QDBusMessage::createMethodCall(dbus_target::name, dbus_target::path,
                                                              dbus_target::interface, method);
        QList<QVariant> arguments;
        arguments << QVariant::fromValue(true);
        call.setArguments(arguments);

        QDBusPendingReply<QVariantMap> response = connection.asyncCall(call);
        response.waitForFinished();
        return responseHandler(std::move(response.reply()));
}

/* private */
PowerActionManager::PowerActionManager() :
        connection(QDBusConnection::connectToBus(QDBusConnection::SystemBus, dbus_target::name)),
        proxy(dbus_target::name, dbus_target::path, dbus_target::interface, connection, nullptr) {}

QDBusMessage PowerActionManager::responseHandler(QDBusMessage response) const {
        if (response.type() == QDBusMessage::ErrorMessage) {
                qCritical() << "Error sending action. Response: " << response.errorMessage()
                            << Qt::endl;
        } else {
                qInfo() << response << Qt::endl;
        }

        return std::move(response);
}
