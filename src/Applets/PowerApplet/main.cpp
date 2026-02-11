/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

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

#include "Config/TomlConfigParser.h"
#include "Core/Log.h"
#include "UI/PowerMainWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
        // Init
        QApplication app(argc, argv);
        TomlConfigParser::parseConfig();

        PowerMainWindow applet; // QMainWindow -> MainWindow -> PowerMainWindow

        // Debug
        QINFO() << "Applet resolution:" << applet.size();
        QDEBUG() << "Qt Version:" << qVersion();
        QINFO() << Qt::endl;

        // Run
        applet.show();
        return app.exec();
}
