// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "PowerCentralWidget.h"

PowerMainWindow::PowerMainWindow() :
        MainWindow(PowerAppletConfig::get().getWindowProperties()),
        central_widget(new PowerCentralWidget{}) {
        resize(PowerAppletConfig::get().getWindowProperties().getSize());
        setCentralWidget(central_widget);
}

const PowerCentralWidget* PowerMainWindow::getCentralWidget() const {
        return central_widget;
}
