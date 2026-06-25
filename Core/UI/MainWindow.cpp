// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"

#include <QMainWindow>
#include <QSize>
#include <QString>
#include <QWidget>

MainWindow::MainWindow(const QString& title, QSize size, QWidget* central_widget, QWidget* parent) :
        QMainWindow(parent) {
        setWindowTitle(title);
        resize(size);
        setCentralWidget(central_widget);
}
