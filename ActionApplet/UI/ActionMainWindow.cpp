// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionMainWindow.h"
#include "ActionApplet/UI/Widgets/ActionButton.h"
#include "ActionCentralWidget.h"
#include "Core/Config/ConfigFile/ActionApplet/ActionAppletConfig.h"
#include "Core/Config/KeysFile/ActionApplet/ActionAppletKeys.h"
#include "Core/UI/MainWindow.h"

#include <utility>
#include <vector>
#include <QWidget>

ActionMainWindow::ActionMainWindow(std::vector<ActionButton*> buttons,
                                   const ActionAppletConfig& config, const ActionAppletKeys& keys,
                                   const ActionAppletKeys& default_keys, QWidget* parent) :
        MainWindow(config.getWindowProperties(), parent) {
        resize(config.getWindowProperties().getSize());
        setCentralWidget(
                new ActionCentralWidget(std::move(buttons), keys.getQuit(),
                                        config.getPrimaryButtonProperties().getDoubleKeyPress(),
                                        this));
}
