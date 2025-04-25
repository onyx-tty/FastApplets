/* Pretty Applets

   Copyright (C) 2025 Łukasz Wrodarczyk

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
#include "API/WeatherParser.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include <array>

// TODO Only accept HourlyWeatherData iterators
template<typename Iterator>
std::array<const float*, 2> findTemperatureRange(const Iterator begin, const Iterator end);

class CellFactory final {
public:
        CellFactory() = delete;

        static QBoxLayout* createCell(QBoxLayout::Direction layout_direction,
                                      const QImage& icon_img, std::optional<const QString> value,
                                      const std::vector<QString> text_list);
};

class CurrentDayLayout final {
public:
        explicit CurrentDayLayout();

        QVBoxLayout* getLayout();

private:
        QVBoxLayout* layout;
};

class CurrentWeekLayout final {
public:
        explicit CurrentWeekLayout();

        void         refreshCells(); // TODO Finish
        QVBoxLayout* getLayout();

private:
        QVBoxLayout* layout;
};

class CentralWidget final : public QWidget {
        Q_OBJECT

public:
        explicit CentralWidget(QWidget* parent, const QApplication& app);

private:
        QHBoxLayout*               main_layout;
        const KeybindingManager    keybindings;
        WeatherParser              weather_parser;
};

#endif // CENTRAL_WIDGET_H
