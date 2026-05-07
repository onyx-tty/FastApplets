// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

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
        const auto connection = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                              target.name);
        const auto proxy = QDBusInterface(target.name, target.path, target.interface, connection,
                                          nullptr);
        if (!proxy.isValid()) { QFATAL("D-Bus proxy is invalid!"); }

        auto call = QDBusMessage::createMethodCall(target.name, target.path, target.interface,
                                                   method);

        call.setArguments({QVariant::fromValue(true)});

        // Avoid overwriting pending_reply mid-work
        waitForFinished();
        pending_reply = connection.asyncCall(call);
}

void DBusRequester::waitForFinished() {
        pending_reply.waitForFinished();
}

const QDBusPendingReply<QVariantMap>& DBusRequester::getReply() {
        waitForFinished();
        return pending_reply;
}
