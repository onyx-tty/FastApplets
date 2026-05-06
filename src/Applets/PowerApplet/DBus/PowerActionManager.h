// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QString>
#include <QVariantMap>

class PowerActionManager final {
private:
        static void printReply(const QDBusPendingReply<QVariantMap>& response);

public:
        explicit PowerActionManager() = delete;
        // TODO Check for validity of power action
        // org.freedesktop.login1 — The D-Bus interface of systemd-logind
        static void sendPowerAction(const QString& method);
};
