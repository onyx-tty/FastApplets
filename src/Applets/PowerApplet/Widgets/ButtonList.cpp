#include "ButtonList.h"
#include "../../../Config/PowerLayout.h"

PowerButton* power_button::shutdown = nullptr;
PowerButton* power_button::reboot = nullptr;
PowerButton* power_button::suspend = nullptr;
PowerButton* power_button::hibernate = nullptr;

void power_button::initAll(QWidget* parent, QVBoxLayout* layout) {
        power_button::shutdown = new PowerButton(parent, layout, icon::shutdown, text::shutdown);
        power_button::reboot = new PowerButton(parent, layout, icon::reboot, text::reboot);
        power_button::suspend = new PowerButton(parent, layout, icon::suspend, text::suspend);
        power_button::hibernate = new PowerButton(parent, layout, icon::hibernate, text::hibernate);
}
