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

#include "KeyAction.h"
#include "Config/Config.h"

#include <QDebug>
#include <qnamespace.h>
#include <QString>

#include <unordered_map>

using std::vector, std::unordered_map;

KeyAction::KeyAction(int key, PowerButton* button) : key(key), button(button) {}

void KeyAction::reset() {
        key    = Qt::Key_unknown;
        button = nullptr;
}

void KeyAction::updatePowerButton(const vector<PowerButton*>& buttons) {
        const auto& button_properties = Config::WindowLayoutProperties::getPrimaryPowerButtons();

        if (button_properties.size() != buttons.size()) {
                qFatal("%s: button_properties (%s) larger than buttons (%s)!", __func__,
                       QString::number(button_properties.size()).toStdString().c_str(),
                       QString::number(buttons.size()).toStdString().c_str());
        }

        static const auto power_key_interpreter = [](int key) -> int {
                static const unordered_map<int, int> power_key_map{
                        {Qt::Key_1, 1},
                        {Qt::Key_2, 2},
                        {Qt::Key_3, 3},
                        {Qt::Key_4, 4}
                };

                return power_key_map.at(key);
        };

        for (size_t i = 0; i != buttons.size(); ++i) {
                if (power_key_interpreter(key) == button_properties[i].order) {
                        button = buttons[i];
                        qDebug() << "Button" << i << "updated to: " << button->getDBusAction();
                        return;
                }
        }

        qFatal("%s: button not found!", __func__);
}
