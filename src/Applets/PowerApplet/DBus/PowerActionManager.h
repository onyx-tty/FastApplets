#ifndef POWER_ACTION_MANAGER_H
#define POWER_ACTION_MANAGER_H

#include <QtDBus/QtDBus> // TODO Don't include directly
#include <QString>

// org.freedesktop.login1 — The D-Bus interface of systemd-logind
namespace target {
extern const QString name;
extern const QString path;
extern const QString interface;
}

class PowerActionManager { // Singleton

public:
        PowerActionManager(const PowerActionManager&) = delete;
        PowerActionManager& operator=(const PowerActionManager&) = delete;
        static PowerActionManager& getInstance(QObject* parent);

        // Power actions
        void shutdown() const;
        void reboot() const;
        void suspend() const;
        void hibernate() const;

private:
        QDBusConnection connection;
        QDBusInterface proxy;
        static PowerActionManager* instance;

        // Methods
        explicit PowerActionManager(QObject* parent = nullptr);

        QDBusMessage sendPowerAction(const QString& method) const;
        QDBusMessage responseHandler(QDBusMessage response) const; // does nothing for now

};

#endif // POWER_ACTION_MANAGER_H
