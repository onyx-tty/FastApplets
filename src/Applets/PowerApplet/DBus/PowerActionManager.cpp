// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerActionManager.h"
#include "CppUtils/Log/QtLog.h"
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
