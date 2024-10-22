#include "PowerMainWindow.h"

#include <QApplication>
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
