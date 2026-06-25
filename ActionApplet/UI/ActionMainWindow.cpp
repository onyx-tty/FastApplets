// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionMainWindow.h"
#include "ActionApplet/UI/Widgets/ActionButton.h"
#include "ActionCentralWidget.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/MainWindow.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QWidget>

ActionMainWindow::ActionMainWindow(const QString& title, QSize size,
                                   std::vector<ActionButton*> buttons, const keybindings& quit_keys,
                                   bool double_key_press, QWidget* parent) :
        MainWindow(title, size, parent) {
        resize(size);
        setCentralWidget(
                new ActionCentralWidget(std::move(buttons), quit_keys, double_key_press, this));
}
