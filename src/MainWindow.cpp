 /* Pretty Applets

    Copyright (C) 2024 Łukasz Wrodarczyk

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "MainWindow.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent) {
        initWindow();
}

MainWindow::~MainWindow() = default;

QWidget* MainWindow::returnCentralWidget() const {
        return central_widget;
}

QVBoxLayout* MainWindow::returnMainLayout() const {
        return main_layout;
}

QSize MainWindow::returnWindowSize() const {
        return QWidget::size();
}

// Main window params
void MainWindow::initWindow() {
        resize(280, 540);
        setWindowTitle("test_window");
        central_widget = new QWidget(this);
        setCentralWidget(central_widget);
        main_layout = new QVBoxLayout(central_widget);
}
