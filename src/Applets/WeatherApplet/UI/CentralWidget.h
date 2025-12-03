/* FastApplets

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

#pragma once

#include <QApplication>
#include <QBoxLayout>
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
        explicit CurrentDayLayout() = delete;

        static QVBoxLayout* getLayout();

private:
        static QVBoxLayout* layout;
};

class CurrentWeekLayout final : public QWidget {
public:
        explicit CurrentWeekLayout() = delete;

        static void         refreshCells(); // TODO Finish
        static QVBoxLayout* getLayout();

private:
        static QVBoxLayout* layout;
};

class CentralWidget final : public QWidget {
        Q_OBJECT
public:
        explicit CentralWidget(QWidget* parent);

        QHBoxLayout* getMainLayout();

private:
        QHBoxLayout* main_layout;
};
