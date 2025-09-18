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

#include "Widgets/PowerButton.h"
#include "../../Config/PowerKeybindings.h"

#include <QHBoxLayout>
#include <QWidget>

#include <array>

struct Action {
        int          key;
        PowerButton* button;

        Action(int key = Qt::Key_unknown, PowerButton* button = nullptr);

        void clear();
};

class CentralWidget final : public QWidget {
        Q_OBJECT

public:
        QBoxLayout* main_layout;

        explicit CentralWidget(QWidget* parent, const QApplication& app);

        void keyPressEvent(QKeyEvent* event) override;
        void lastKeyUpdateEvent(QKeyEvent* event);
        void keyPressEvent(QKeyEvent* event, PowerButton* button);
        void selectButton(QKeyEvent* event, PowerButton* button,
                          std::array<int, 4>& acceptable_keys);
        void clickButton(QKeyEvent* event);

private:
        Action                             last_action;
        Action                             current_action;
        const std::array<PowerButton*, 4>* button_list;
        const PowerKeybindingManager       keybindings;

        // TODO Optimize
        void updateActions();
        void resetActions();
};

#endif // CENTRAL_WIDGET_H
