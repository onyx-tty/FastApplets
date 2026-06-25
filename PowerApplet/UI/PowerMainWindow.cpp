// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/MainWindow.h"
#include "PowerApplet/UI/Widgets/PowerButton.h"
#include "PowerCentralWidget.h"

#include <utility>
#include <vector>
#include <QSize>
#include <QWidget>

PowerMainWindow::PowerMainWindow(const QString& title, QSize size,
                                 std::vector<PowerButton*> buttons, const keybindings& quit_keys,
                                 bool double_key_press, QWidget* parent) :
        MainWindow(title, size, parent) {
        setCentralWidget(
                new PowerCentralWidget(std::move(buttons), quit_keys, double_key_press, this));
}
