#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow
	: public QMainWindow {
	Q_OBJECT
        QWidget* center;
        QVBoxLayout* main_layout;
private:
        void initWindow();
        void initWidgets();

public:
        explicit MainWindow(QWidget* parent = nullptr);

public:
        QWidget* returnCentralWidget() const;
        QSize returnWindowSize() const;
};


#endif // MAIN_WINDOW_H
