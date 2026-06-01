// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Config/Manager/ConfigManager.h"
#include "PowerCentralWidget.h"
#include "UI/MainWindow.h"

#include <QWidget>

PowerMainWindow::PowerMainWindow(QWidget* parent) :
        MainWindow(ConfigManager::getConfig().getWindowProperties(), parent) {
        resize(ConfigManager::getConfig().getWindowProperties().getSize());
        setCentralWidget(new PowerCentralWidget(this));
}
