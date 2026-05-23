// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "PowerCentralWidget.h"
#include "UI/MainWindow.h"

#include <QWidget>

PowerMainWindow::PowerMainWindow(QWidget* parent) :
        MainWindow(PowerAppletConfig::get().getWindowProperties(), parent) {
        resize(PowerAppletConfig::get().getWindowProperties().getSize());
        setCentralWidget(new PowerCentralWidget(this));
}
