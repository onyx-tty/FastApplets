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

#include "CentralWidget.h"
#include "Utils/PresentTime.h"

#include <QDebug>
#include <QLabel>

template<typename Iterator>
std::array<const float*, 2> findTemperatureRange(const Iterator begin, const Iterator end) {
        if (!begin) {
                qCritical() << "Encountered null iterator in" << __func__ << ", quitting!";
                QApplication::quit();
        }
        std::array<const float*, 2> range{&begin->temperature, &begin->temperature};
        for (auto it = begin; it != end; ++it) {
                if (it->temperature < *range[0]) range[0] = &it->temperature; // min
                if (it->temperature > *range[1]) range[1] = &it->temperature; // max
        }

        return std::move(range);
}

QBoxLayout* CellFactory::createCell(QBoxLayout::Direction layout_direction, const QImage& icon_img,
                                    std::optional<const QString> value_text,
                                    const std::vector<QString>   text_list) {
        bool        has_value      = value_text.has_value();
        // add new cell, split into three sides
        QBoxLayout* cell           = new QBoxLayout(layout_direction);
        QBoxLayout* cell_icon_side = new QBoxLayout(layout_direction);
        cell->addLayout(cell_icon_side);
        QBoxLayout* cell_value_side = nullptr;
        if (has_value) {
                cell_value_side = new QBoxLayout(layout_direction);
                cell->addLayout(cell_value_side);
        }
        QHBoxLayout* cell_text_side = new QHBoxLayout;
        cell->addLayout(cell_text_side);

        // add the icon
        QLabel* icon = new QLabel(cell_icon_side->widget());
        icon->setPixmap(QPixmap::fromImage(icon_img));
        cell_icon_side->addWidget(icon);

        // add the value
        if (has_value) {
                QLabel* value = new QLabel(cell_value_side->widget());
                value->setText(std::move(value_text.value()));
                cell_value_side->addWidget(value);
        }
        // add text fields
        for (auto text_element : text_list) {
                QLabel* text = new QLabel(cell_text_side->widget());
                text->setText(std::move(text_element));
                cell_text_side->addWidget(text);
        }

        return cell;
}

CurrentDayLayout::CurrentDayLayout() : layout(new QVBoxLayout) {
        auto alignment = QBoxLayout::LeftToRight;
        auto now       = WeatherData::hours.cbegin();

        // weather
        layout->addLayout(CellFactory::createCell(alignment, now->weather->day_icon, {},
                                                  {now->weather->detailed_name}));
        // temperature
        layout->addLayout(CellFactory::createCell(alignment, QImage(),
                                                  QString::number(now->temperature),
                                                  {"Temperature"}));
        // city name
        // TODO Fetch it from the API call, it's going to be empty until then
        layout->addLayout(CellFactory::createCell(alignment, QImage(), {}, {now->city_name}));
        // pressure
        layout->addLayout(CellFactory::createCell(alignment, QImage(),
                                                  QString::number(now->atmospheric_pressure),
                                                  {"Pressure"}));
        // wind speed
        layout->addLayout(CellFactory::createCell(alignment, QImage(),
                                                  QString::number(now->wind_speed), {"Wind"}));
        // humidity
        layout->addLayout(CellFactory::createCell(alignment, QImage(),
                                                  QString::number(now->humidity), {"Humidity"}));
        // rain
        layout->addLayout(
                CellFactory::createCell(alignment, QImage(), QString::number(now->rain), {"Rain"}));
}

QVBoxLayout* CurrentDayLayout::getLayout() {
        if (!layout) {
                qFatal("CurrentDayLayout's layout is nullptr, quitting!");
                QApplication::quit();
        }

        return layout;
}

CurrentWeekLayout::CurrentWeekLayout() : layout(new QVBoxLayout) {
        // TODO Replace certain elements with WeatherData::fillDayNames
        constexpr auto cell_alignment  = QBoxLayout::TopToBottom;
        const auto     iter_begin      = WeatherData::hours.cbegin();
        const auto     iter_end        = WeatherData::hours.cend();
        const auto&    blocs_per_day   = PresentTimeManager::getBlocsPerDay();
        const auto&    first_day_blocs = PresentTimeManager::getFirstDayBlocs().value_or(0);
        const auto&    last_day_blocs  = PresentTimeManager::getLastDayBlocs();
        using Range                    = std::array<const float*, 2>;
        auto formatTemperatureRange    = [](const Range& temperature_range) -> const QString {
                return QString::number(*temperature_range[0]) + "-"
                     + QString::number(*temperature_range[1]);
        };

        // First, we set up the range of blocs to be iterated over for the first day
        // Next, we create a cell for that potentially incomplete day
        std::pair<const HourlyWeatherData*, const HourlyWeatherData*>
                bloc_border{iter_begin, (iter_begin + first_day_blocs)};
        Range   temperature_range = findTemperatureRange(bloc_border.first, bloc_border.second);
        layout->addLayout(CellFactory::createCell(cell_alignment, QImage(),
                                                  formatTemperatureRange(temperature_range),
                                                  {bloc_border.first->day}));

        // We're done with day 1, moving forward we expect a range with the length of blocs_per_day
        bloc_border.first  = bloc_border.second + 1;
        bloc_border.second = bloc_border.first + blocs_per_day;

        // Then we iterate over the blocs in-between the first and last days
        // creating cells along the way
        for (; bloc_border.second + blocs_per_day < iter_end;
             bloc_border.first += blocs_per_day, bloc_border.second += blocs_per_day) {
                temperature_range = findTemperatureRange(bloc_border.first, bloc_border.second);
                layout->addLayout(CellFactory::createCell(cell_alignment, QImage(),
                                                          formatTemperatureRange(temperature_range),
                                                          {bloc_border.first->day}));

                // Keep iterating until adding blocs_per_day makes us go beyond iter_end
                if (bloc_border.second + blocs_per_day < iter_end) {
                        continue;
                } else if (first_day_blocs != blocs_per_day) {
                        bloc_border.first  = bloc_border.second + 1;
                        bloc_border.second = bloc_border.first + last_day_blocs;
                        if (bloc_border.second == iter_end) {
                                qDebug() << "bloc_border.second is iter_end (one past the list)";
                        }
                        if (bloc_border.second == nullptr) {
                                qCritical() << "bloc_border.second is null! that's dangerous!";
                        }
                        temperature_range = findTemperatureRange(bloc_border.first,
                                                                 bloc_border.second);
                        layout->addLayout(
                                CellFactory::createCell(cell_alignment, QImage(),
                                                        formatTemperatureRange(temperature_range),
                                                        {bloc_border.first->day}));
                }
        }
        qDebug() << "DONE!";
}

void CurrentWeekLayout::refreshCells() {
        using Range                 = std::array<const float*, 2>;
        auto formatTemperatureRange = [](const Range& temperature_range) -> const QString {
                return QString::number(*temperature_range[0]) + "-"
                     + QString::number(*temperature_range[1]);
        };
        // TODO Finish later
}

QVBoxLayout* CurrentWeekLayout::getLayout() {
        if (!layout) {
                qFatal("CurrentWeekLayout's layout is nullptr, quitting!");
                QApplication::quit();
        }

        return layout;
}

CentralWidget::CentralWidget(QWidget* parent, const QApplication& app) :
        QWidget(parent), main_layout(new QHBoxLayout(this)), weather_parser(this, app) {
        WeatherLayoutManager::setup(app); // We give WeatherLayoutManager access to runtime
        weather_parser.updateWeatherData();
        // Split main layout
        auto* left_layout  = new QVBoxLayout;
        auto* right_layout = new QVBoxLayout;
        main_layout->addLayout(left_layout);
        main_layout->addLayout(right_layout);
        // Parts of left layout
        CurrentDayLayout  current_day_layout;
        CurrentWeekLayout current_week_layout;
        left_layout->addLayout(current_day_layout.getLayout());
        left_layout->addLayout(current_week_layout.getLayout());
};
