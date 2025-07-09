#ifndef BUTTON_LIST_H
#define BUTTON_LIST_H

#include "../Widgets/PowerButton.h"

#include <QString>

namespace power_button { // pointers to heap-allocated Buttons
extern PowerButton* shutdown;
extern PowerButton* reboot;
extern PowerButton* suspend;
extern PowerButton* hibernate;
// TODO Make this more layout-agnostic once the layout attribute is moved from
//      MainWindow to PowerMainWindow
extern void initAll(QWidget* parent, QVBoxLayout* layout); // initializes heap-allocated buttons
}                                                          // from the pointers above

#endif // BUTTON_LIST_H
