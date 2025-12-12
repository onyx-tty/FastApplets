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

#include "Input/KeyAction.h"
#include "Widgets/PowerButton.h"

#include <qnamespace.h>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QWidget>

#include <vector>

class PowerCentralWidget final : public QWidget {
        Q_OBJECT

public:
        QBoxLayout* main_layout;

        explicit PowerCentralWidget(QWidget* parent);

        void keyPressEvent(QKeyEvent* event) override;
        void lastKeyUpdateEvent(QKeyEvent* event);
        void keyPressEvent(QKeyEvent* event, PowerButton* button);
        void selectButton(QKeyEvent* event, PowerButton* button,
                          std::array<int, 4>& acceptable_keys);
        void clickButton(QKeyEvent* event);

private:
        KeyAction                       last_action;
        KeyAction                       current_action;
        const std::vector<PowerButton*> button_list;

        // TODO Optimize
        void updateActions();
        void resetActions();
};
