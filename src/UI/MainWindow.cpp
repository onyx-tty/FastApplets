// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <QMainWindow>

MainWindow::MainWindow(const WindowProperties& properties, QWidget* parent) : QMainWindow(parent) {
        resize(properties.getSize());
        setWindowTitle(properties.getTitle());
}

MainWindow::~MainWindow() = default;
