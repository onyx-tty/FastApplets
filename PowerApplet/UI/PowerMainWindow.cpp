// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
// TODO: Create a template specialization for AppletManager,
//       removing the need for external includes
// Pull in definitions for ConfigManager<PowerApplet> overload
#include "PowerApplet/Types/PowerAppletTraits.h"

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Manager/ConfigManager.h"
#include "PowerCentralWidget.h"
#include "Core/UI/MainWindow.h"

#include <QWidget>

// TODO: Accept properties as param for dependency injection
PowerMainWindow::PowerMainWindow(QWidget* parent) :
        MainWindow(ConfigManager<applet::type::power_applet>::getConfig().getWindowProperties(),
                   parent) {
        resize(ConfigManager<applet::type::power_applet>::getConfig()
                       .getWindowProperties()
                       .getSize());
        setCentralWidget(new PowerCentralWidget(this));
}
