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

#ifndef POWER_MAIN_WINDOW_H
#define POWER_MAIN_WINDOW_H

#include "../../MainWindow.h"
#include "CentralWidget.h"

class PowerMainWindow final
        : public MainWindow {
        Q_OBJECT

public:
        PowerMainWindow(QWidget* parent = nullptr);

private:
        CentralWidget* central_widget;

};

#endif // POWER_MAIN_WINDOW_H
