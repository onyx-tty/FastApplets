// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionMainWindow.h"
#include "ActionCentralWidget.h"
#include "Core/Config/ConfigFile/ActionApplet/ActionAppletConfig.h"
#include "Core/Config/KeysFile/ActionApplet/ActionAppletKeys.h"
#include "Core/UI/MainWindow.h"

#include <QWidget>

ActionMainWindow::ActionMainWindow(const ActionAppletConfig& config, const ActionAppletKeys& keys,
                                   const ActionAppletKeys& default_keys, QWidget* parent) :
        MainWindow(config.getWindowProperties(), parent) {
        resize(config.getWindowProperties().getSize());
        setCentralWidget(new ActionCentralWidget(config, keys, default_keys, this));
}
