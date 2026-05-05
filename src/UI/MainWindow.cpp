// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

MainWindow::MainWindow(const WindowProperties& properties) {
        resize(properties.getSize());
        setWindowTitle(properties.getTitle());
}

MainWindow::~MainWindow() = default;
