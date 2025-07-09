#ifndef POWER_BUTTON_H
#define POWER_BUTTON_H

#include "../../../Widgets/Button.h"

class PowerButton final
        : public Button {
        Q_OBJECT

public:
        PowerButton(QWidget* parent, QVBoxLayout* main_layout, QIcon button_icon, QString text)
                : Button(parent, main_layout, button_icon, text) {}
};

#endif // POWER_BUTTON_H
