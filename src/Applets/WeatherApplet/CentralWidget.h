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

#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include "../../Config/DefaultKeybindings.h"
#include "../../Config/DefaultLayout.h"
#include "API/OpenWeather.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

class CentralWidget final : public QWidget {
        Q_OBJECT

public:
        explicit CentralWidget(QWidget* parent, QApplication* app);

private:
        QHBoxLayout*      main_layout;
        LayoutManager     layout;
        KeybindingManager keybindings;
        OpenWeatherAPI    open_weather_api;
};

#endif // CENTRAL_WIDGET_H
