#include "PowerMainWindow.h"
#include "Widgets/ButtonList.h"

PowerMainWindow::PowerMainWindow(QWidget* parent)
        : MainWindow(parent) {
        initWidgets();
}

void PowerMainWindow::initWidgets() {
        // TODO There's probably a better way to call that namespace
        power_button::initAll(returnCentralWidget(), returnMainLayout());
}
