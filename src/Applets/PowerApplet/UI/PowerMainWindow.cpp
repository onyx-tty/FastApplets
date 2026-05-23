// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "PowerCentralWidget.h"
#include "UI/MainWindow.h"

PowerMainWindow::PowerMainWindow() : MainWindow(PowerAppletConfig::get().getWindowProperties()) {
        resize(PowerAppletConfig::get().getWindowProperties().getSize());
        setCentralWidget(new PowerCentralWidget());
}
