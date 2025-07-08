#include "MainWindow.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent) {
        initWindow();
}

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
