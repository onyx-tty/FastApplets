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

#include "PowerActionManager.h"

#include <QDebug>

const QString target::name{"org.freedesktop.login1"};
const QString target::path{"/org/freedesktop/login1"};
const QString target::interface{"org.freedesktop.login1.Manager"};

/* public */
PowerActionManager& PowerActionManager::getInstance() {
        static PowerActionManager instance;
        return instance;
}

QDBusMessage PowerActionManager::sendPowerAction(const QString& method) const {
        if (!proxy.isValid()) {
                qFatal("Proxy is invalid!\n");
                return QDBusMessage();
        }

        QDBusMessage method_call = QDBusMessage::createMethodCall(target::name,
                                                                  target::path,
                                                                  target::interface,
                                                                  method);
        QList<QVariant> arguments;
        arguments << QVariant::fromValue(true);
        method_call.setArguments(arguments);

        QDBusPendingReply<QVariantMap> response = connection.asyncCall(method_call);
        response.waitForFinished();
        return responseHandler(response.reply());
}

/* private */
PowerActionManager::PowerActionManager(QObject* parent)
        : connection(QDBusConnection::connectToBus(QDBusConnection::SystemBus, target::name)),
          proxy(target::name, target::path, target::interface, connection, parent) {}

QDBusMessage PowerActionManager::responseHandler(QDBusMessage response) const {
        if (response.type() == QDBusMessage::ErrorMessage) {
                qCritical() << "Error sending action. Response: " << response.errorMessage()
                            << Qt::endl;
        } else {
                qInfo() << response << Qt::endl;
        }

        return response;
}
