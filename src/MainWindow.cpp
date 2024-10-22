#include "MainWindow.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) {
        initWindow();
        initWidgets();
}

void MainWindow::initWindow() { // Main window params
        resize(280, 540);
        setWindowTitle("test_window");
        center = new QWidget(this);
        setCentralWidget(center);
        main_layout = new QVBoxLayout(center);
}

void MainWindow::initWidgets() { // Widgets to be included
        // ...
}

QSize MainWindow::returnWindowSize() const {
        return QWidget::size();
}

QWidget* MainWindow::returnCentralWidget() const {
        return center;
}
