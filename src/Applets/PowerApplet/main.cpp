#include "../../MainWindow.h"

#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QDebug>

int main(int argc, char* argv[]) {
        QApplication app(argc, argv);
        MainWindow applet;

        // Debug
        qInfo() << applet.returnWindowSize() << Qt::endl;

        // Run
        applet.show();

        return app.exec();
}
