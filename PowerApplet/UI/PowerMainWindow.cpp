// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Core/Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Core/Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Core/UI/MainWindow.h"
#include "PowerCentralWidget.h"

#include <QWidget>

PowerMainWindow::PowerMainWindow(const PowerAppletConfig& config, const PowerAppletKeys& keys,
                                 const PowerAppletKeys& default_keys, QWidget* parent) :
        MainWindow(config.getWindowProperties(), parent) {
        resize(config.getWindowProperties().getSize());
        setCentralWidget(new PowerCentralWidget(config, keys, default_keys, this));
}
