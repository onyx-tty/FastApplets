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
#include "../../Config/WeatherLayout.h"
#include "API/WeatherParser.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

class CentralWidget final : public QWidget {
        Q_OBJECT

public:
        explicit CentralWidget(QWidget* parent, QApplication* app);

private:
        QHBoxLayout* main_layout;
        /*-----------< MAIN SPLIT >------------*/
        QVBoxLayout* current_day_layout;
        QVBoxLayout* auxiliary_layout;
        /*-----------< LEFT SPLIT >------------*/
        /*		  		       */
        /*-----------< RIGHT SPLIT >-----------*/
        QVBoxLayout* details_layout;
        QVBoxLayout* time_passage_layout;
        /*-------------------------------------*/

        const WeatherLayoutManager layout;
        const KeybindingManager    keybindings;
        WeatherParser              weather_parser;
};

#endif // CENTRAL_WIDGET_H
