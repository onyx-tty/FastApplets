#include "ButtonList.h"
#include "../DBus/PowerActionManager.h"
#include "../../../Config/PowerLayout.h"

#include <QDebug>

PowerButton* power_button::shutdown = nullptr;
PowerButton* power_button::reboot = nullptr;
PowerButton* power_button::suspend = nullptr;
PowerButton* power_button::hibernate = nullptr;

void power_button::initAll(QWidget* parent, QVBoxLayout* layout) {
        power_button::shutdown = new PowerButton(parent, layout, icon::shutdown, text::shutdown);
        PowerButton::connect(power_button::shutdown, &PowerButton::clicked, []() {
                qInfo() << "Initiating shutdown...\n";
                PowerActionManager::getInstance().shutdown();
        });

        power_button::reboot = new PowerButton(parent, layout, icon::reboot, text::reboot);
        PowerButton::connect(power_button::reboot, &PowerButton::clicked, []() {
                qInfo() << "Initiating reboot...\n";
                PowerActionManager::getInstance().reboot();
        });

        power_button::suspend = new PowerButton(parent, layout, icon::suspend, text::suspend);
        PowerButton::connect(power_button::suspend, &PowerButton::clicked, []() {
                qInfo() << "Initiating suspension...\n";
                PowerActionManager::getInstance().suspend();
        });

        power_button::hibernate = new PowerButton(parent, layout, icon::hibernate, text::hibernate);
        PowerButton::connect(power_button::hibernate, &PowerButton::clicked, []() {
                qInfo() << "Initiating hibernation...\n";
                PowerActionManager::getInstance().hibernate();
        });
}
