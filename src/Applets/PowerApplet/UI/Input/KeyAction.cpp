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

namespace {
static const unordered_map<int, int> power_key_map{{Qt::Key_1, 1},
                                                   {Qt::Key_2, 2},
                                                   {Qt::Key_3, 3},
                                                   {Qt::Key_4, 4}};
}

KeyAction::KeyAction() : key(Qt::Key_unknown), button(nullptr) {}

KeyAction::KeyAction(int key, PowerButton* button) : key(key), button(button) {}

KeyAction::KeyAction(const KeyAction& other) {
        key    = other.key;
        button = other.button;
}

KeyAction::KeyAction(KeyAction&& other) {
        key    = other.key;
        button = other.button;
}

KeyAction& KeyAction::operator=(const KeyAction& other) {
        if (this != &other) {
                key    = other.key;
                button = other.button;
        }

        return *this;
}

KeyAction& KeyAction::operator=(KeyAction&& other) {
        if (this != &other) {
                key    = other.key;
                button = other.button;
        }

        return *this;
}

int KeyAction::getKey() const {
        return key;
}

const PowerButton* KeyAction::getButton() const {
        return button;
}

PowerButton* KeyAction::debugGetButtonNonConst() {
        return button;
}

void KeyAction::updatePowerButton(const vector<PowerButton*>& buttons) {
        const auto& button_properties = Config::WindowLayoutProperties::getPrimaryPowerButtons();

        if (button_properties.size() != buttons.size()) {
                qFatal("%s: button_properties (%s) larger than buttons (%s)!", __func__,
                       QString::number(button_properties.size()).toStdString().c_str(),
                       QString::number(buttons.size()).toStdString().c_str());
        }

        for (size_t i = 0; i != buttons.size(); ++i) {
                if (power_key_map.at(key) == button_properties[i].order) {
                        button = buttons[i];
                        qDebug() << "Button" << i << "updated to: " << button->getDBusAction();
                        return;
                }
        }

        qFatal("%s: button not found!", __func__);
}

void KeyAction::reset() {
        key    = Qt::Key_unknown;
        button = nullptr;
}
