#include "PowerMainWindow.h"

#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QDebug>

int main(int argc, char* argv[]) {
        // Init
        QApplication app(argc, argv);
        PowerMainWindow applet; // QMainWindow -> MainWindow -> PowerMainWindow

        // Debug
        qInfo() << applet.returnWindowSize() << Qt::endl;

        // Run
        applet.show();

        return app.exec();
}
