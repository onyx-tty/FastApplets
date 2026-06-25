// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Core/Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Core/Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Core/UI/MainWindow.h"
#include "PowerApplet/UI/Widgets/PowerButton.h"
#include "PowerCentralWidget.h"

#include <utility>
#include <vector>
#include <QWidget>

PowerMainWindow::PowerMainWindow(std::vector<PowerButton*> buttons, const PowerAppletConfig& config,
                                 const PowerAppletKeys& keys, const PowerAppletKeys& default_keys,
                                 QWidget* parent) :
        MainWindow(config.getWindowProperties(), parent) {
        resize(config.getWindowProperties().getSize());
        setCentralWidget(
                new PowerCentralWidget(std::move(buttons), keys.getQuit(),
                                       config.getPrimaryButtonProperties().getDoubleKeyPress(),
                                       this));
}
