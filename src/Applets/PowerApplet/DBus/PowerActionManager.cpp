#include "PowerActionManager.h"

#include <QDebug>

const QString target::name{"org.freedesktop.login1"};
const QString target::path{"/org/freedesktop/login1"};
const QString target::interface{"org.freedesktop.login1.Manager"};

/* public */
PowerActionManager& PowerActionManager::getInstance(QObject* parent) {
        if (instance == nullptr) {
                instance = new PowerActionManager(parent);
        }
        return *instance;
}

void PowerActionManager::shutdown() const {
        sendPowerAction("PowerOff");
}

void PowerActionManager::reboot() const {
        sendPowerAction("Reboot");
}

void PowerActionManager::suspend() const {
        sendPowerAction("Suspend");
}

void PowerActionManager::hibernate() const {
        sendPowerAction("Hibernate");
}

/* private */
PowerActionManager::PowerActionManager(QObject* parent)
        : connection(QDBusConnection::connectToBus(QDBusConnection::SystemBus, target::name)),
          proxy(target::name, target::path, target::interface, connection, parent) {}

QDBusMessage PowerActionManager::sendPowerAction(const QString& method) const {
        if (!proxy.isValid()) { // TODO Error handling
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
        //return responseHandler(response);
}

QDBusMessage PowerActionManager::responseHandler(QDBusMessage response) const {
        if (response.type() == QDBusMessage::ErrorMessage) {
                qCritical() << "Error sending action. Response: " << response.errorMessage() << Qt::endl;
        }
        return response;
}

// static variables defined here
PowerActionManager* PowerActionManager::instance = nullptr;
