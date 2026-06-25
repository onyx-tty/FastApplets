// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"

#include <QMainWindow>
#include <QSize>
#include <QString>

MainWindow::MainWindow(const QString& title, QSize size, QWidget* parent) : QMainWindow(parent) {
        setWindowTitle(title);
        resize(size);
}

MainWindow::~MainWindow() = default;
