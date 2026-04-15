/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

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

#include "UI/Widgets/PrimaryButtonParams.h"

class ShellCommand;
class QString;

struct PowerButtonParams final : PrimaryButtonParams {
        power_button_id id;
        ShellCommand    command;
        QString         dbus_method;

        PowerButtonParams(power_button_id id = power_button_id::none, QString label = {},
                          long order = {}, ShellCommand command = {}, QString dbus_method = {}) :
                PrimaryButtonParams(std::move(label), order), id(id), command(std::move(command)),
                dbus_method(std::move(dbus_method)) {}
};
