 /* Pretty Applets

    Copyright (C) 2024 Łukasz Wrodarczyk

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

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
