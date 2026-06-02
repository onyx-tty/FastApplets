// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
// Pull in definitions for ConfigManager<PowerApplet> overload
#include "Applets/PowerApplet/Types/PowerAppletTraits.h"

#include "Applets/Types/AppletType.h"
#include "Config/Manager/ConfigManager.h"
#include "PowerCentralWidget.h"
#include "UI/MainWindow.h"

#include <QWidget>

PowerMainWindow::PowerMainWindow(QWidget* parent) :
        MainWindow(ConfigManager<applet::type::power_applet>::getConfig().getWindowProperties(),
                   parent) {
        resize(ConfigManager<applet::type::power_applet>::getConfig()
                       .getWindowProperties()
                       .getSize());
        setCentralWidget(new PowerCentralWidget(this));
}
