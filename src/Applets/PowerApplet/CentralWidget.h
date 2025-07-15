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

#include "Widgets/PowerButton.h"
#include "../../Config/PowerLayout.h"
#include "../../Config/PowerKeybindings.h"

#include <QHBoxLayout>
#include <QWidget>

#include <array>
#include <utility>

class CentralWidget final : public QWidget {
        Q_OBJECT

public:
        QHBoxLayout* main_layout;

        explicit CentralWidget(QWidget* parent);

        void keyPressEvent(QKeyEvent* event) override;
        void lastKeyUpdateEvent(QKeyEvent* event);
        void keyPressEvent(QKeyEvent* event, PowerButton* button);
        void selectButton(QKeyEvent* event,
                                        PowerButton* button,
                                        std::array<unsigned, 4>& acceptable_keys);
        void clickButton(QKeyEvent* event);

private:
        std::pair<QKeyEvent*, PowerButton*> last_key;
        std::array<PowerButton*, 4>         button_list;
        PowerLayoutManager                  layout;
        PowerKeybindingManager              keybindings;

        void updateLastKey(QKeyEvent* event, PowerButton* button);
        void setPowerButtonStyle(PowerButton* button, const QString style, QKeyEvent* event);
        // receive lambda as a parameter 'action'
        void updatePowerButton(QKeyEvent* event, const QString style,
                               auto&& action);
};

#endif // CENTRAL_WIDGET_H
