// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "DBusTarget.h"

#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QString>
#include <QVariantMap>

class DBusRequester final {
private:
        static QDBusPendingReply<QVariantMap> pending_reply;

public:
        DBusRequester() = delete;

        static void call(const DBusTarget& target, const QString& method);
        static void waitForFinished();
        static const QDBusPendingReply<QVariantMap>& getReply();
};
