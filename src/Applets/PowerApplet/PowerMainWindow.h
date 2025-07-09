#ifndef POWER_MAIN_WINDOW_H
#define POWER_MAIN_WINDOW_H

#include "../../MainWindow.h"

class PowerMainWindow final
        : public MainWindow {
        Q_OBJECT

public:
        PowerMainWindow(QWidget* parent = nullptr);

private:
        void initWidgets();

};

#endif // POWER_MAIN_WINDOW_H
