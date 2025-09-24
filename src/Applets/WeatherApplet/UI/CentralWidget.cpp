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

#include "CentralWidget.h"
#include "../../Config/WeatherLayout.h"
#include "../Time/ForecastBlocManager.h"
#include "../Parsers/OpenWeatherParser.h"

#include <QDebug>
#include <QLabel>

template<typename Iterator>
std::array<const float*, 2> findTemperatureRange(const Iterator begin, const Iterator end) {
        if (begin == end) {
                qCritical() << "Given empty iterator range in" << __func__
                            << "! Accuracy cannot be guaranteed!";
                // TODO std::optional used to properly handle lacking data
                return {0, 0};
        }

        std::array<const float*, 2> range{&begin->temperature, &begin->temperature};
        for (auto it = begin; it != end; ++it) {
                if (it->temperature < *range[0]) range[0] = &it->temperature; // min
                if (it->temperature > *range[1]) range[1] = &it->temperature; // max
        }

        return std::move(range);
}

QBoxLayout* CellFactory::createCell(const QBoxLayout::Direction layout_direction,
                                    const QImage& icon_img, std::optional<const QString> value_text,
                                    const std::vector<QString> text_list) {
        const bool  has_value      = value_text.has_value();
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

QVBoxLayout* CurrentDayLayout::getLayout() {
        if (!layout) {
                layout = new QVBoxLayout;
                const auto alignment = QBoxLayout::LeftToRight;
                auto       now       = WeatherData::hours.cbegin();

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

        return layout;
}

QVBoxLayout* CurrentDayLayout::layout = nullptr;

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
                layout = new QVBoxLayout;
                // TODO Replace certain elements with WeatherData::fillDayNames
                constexpr auto cell_alignment     = QBoxLayout::LeftToRight;
                const auto     begin              = WeatherData::hours.cbegin(), end = WeatherData::hours.cend();
                const auto&    blocs_per_day      = ForecastBlocManager::getBlocsPerDay(),
                               first_day_blocs    = ForecastBlocManager::getFirstDayBlocs().value_or(0),
                               last_day_blocs     = ForecastBlocManager::getLastDayBlocs();
                using Range                       = std::array<const float*, 2>;
                const auto formatTemperatureRange = [](const Range& temperature_range) -> const QString {
                        return QString::number(*temperature_range[0]) + "-"
                             + QString::number(*temperature_range[1]);
                };

                // First, we set up the range of blocs to be iterated over for the first day
                // Next, we create a cell for that potentially incomplete day
                std::pair<const HourlyWeatherData*, const HourlyWeatherData*>
                        bloc_border{begin, (begin + first_day_blocs)};
                Range   temperature_range = findTemperatureRange(bloc_border.first, bloc_border.second);
                layout->addLayout(CellFactory::createCell(cell_alignment, QImage(),
                                                          formatTemperatureRange(temperature_range),
                                                          {bloc_border.first->day}));

                // We're done with day 1, moving forward we expect a range with the length of blocs_per_day
                bloc_border.first  = bloc_border.second + 1;
                bloc_border.second = bloc_border.first + blocs_per_day;

                // Then we iterate over the blocs in-between the first and last days
                // creating cells along the way
                for (; bloc_border.second + blocs_per_day < end;
                     bloc_border.first += blocs_per_day, bloc_border.second += blocs_per_day) {
                        temperature_range = findTemperatureRange(bloc_border.first, bloc_border.second);
                        layout->addLayout(CellFactory::createCell(cell_alignment, QImage(),
                                                                  formatTemperatureRange(temperature_range),
                                                                  {bloc_border.first->day}));

                        // Keep iterating until adding blocs_per_day makes us go beyond iter_end
                        if (bloc_border.second + blocs_per_day < end) {
                                continue;
                        } else if (first_day_blocs != blocs_per_day) {
                                bloc_border.first  = bloc_border.second + 1;
                                bloc_border.second = bloc_border.first + last_day_blocs;
                                temperature_range  = findTemperatureRange(bloc_border.first,
                                                                          bloc_border.second);
                                layout->addLayout(
                                        CellFactory::createCell(cell_alignment, QImage(),
                                                                formatTemperatureRange(temperature_range),
                                                                {bloc_border.first->day}));
                        }
                }
        }

        return layout;
}

QVBoxLayout* CurrentWeekLayout::layout = nullptr;

CentralWidget::CentralWidget(QWidget* parent) :
        QWidget(parent), main_layout(new QHBoxLayout(this)) {
        OpenWeatherParser::updateWeatherData();

        // Split main layout
        auto* left_layout  = new QVBoxLayout;
        auto* right_layout = new QVBoxLayout;

        main_layout->addLayout(left_layout, 1);
        main_layout->addLayout(right_layout, 1);

        // Parts of left layout
        left_layout->addLayout(CurrentDayLayout::getLayout());
        left_layout->addLayout(CurrentWeekLayout::getLayout());
};

QHBoxLayout* CentralWidget::getMainLayout() {
        return main_layout;
}
