#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow
	: public QMainWindow {
	Q_OBJECT

protected:
        QWidget* central_widget;
        QVBoxLayout* main_layout; // Move this to PowerMainWindow.h

        explicit MainWindow(QWidget* parent = nullptr);
        virtual ~MainWindow() = 0;

public:
        QWidget* returnCentralWidget() const;
        QVBoxLayout* returnMainLayout() const; // Move this to PowerMainWindow.h
        QSize returnWindowSize() const;

private:
        void initWindow();
};

#endif // MAIN_WINDOW_H
