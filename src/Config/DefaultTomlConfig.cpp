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

#include "DefaultTomlConfig.h"

#include <string_view>

using namespace std::string_view_literals;

consteval std::string_view createConfig() {
        constexpr auto config = R"(
                [global]
                [global.window]
                size = [960, 220]
                title = "test_window"
                [global.primary_button]
                text_alignment = "top"
                icon_alignment = "center"
                icon_size = [64, 64]
                policy = "expanding"

                [power_applet]
                #[power_applet.window]
                #size = [960, 220]
                #title = "test_window"
                #[power_applet.primary_button]
                #text_alignment = "top"
                #icon_alignment = "center"
                #icon_size = [64, 64]
                #policy = "expanding"
                [power_applet.layout]
                [[power_applet.layout.primary_buttons]]
                id = "shutdown"
                label = "Shutdown"
                order = 1
                enabled = true

                [[power_applet.layout.primary_buttons]]
                id = "reboot"
                label = "Reboot"
                order = 2
                enabled = true

                [[power_applet.layout.primary_buttons]]
                id = "suspend"
                label = "Suspend"
                order = 3
                enabled = true

                [[power_applet.layout.primary_buttons]]
                id = "hibernate"
                label = "Hibernate"
                order = 4
                enabled = true


                #[weather_applet]
                #[weather_applet.window]
                #size = [960, 220]
                #title = "test_window"
                #[weather_applet.primary_button]
                #text_alignment = "top"
                #icon_alignment = "center"
                #icon_size = [64, 64]
                #policy = "expanding"
        )"sv;

        return config;
}

consteval std::string_view createKeys() {
        constexpr auto input = R"(
                [global]
                quit = ["Escape", "Q"]

                [power_applet]
                #quit = ["Escape", "Q"]
                primary_button1 = ["1"]
                primary_button2 = ["2"]
                primary_button3 = ["3"]
                primary_button4 = ["4"]
        )"sv;

        return input;
}

const toml::table& DefaultTomlConfig::getConfig() {
        static const toml::table config = toml::parse(createConfig());

        return config;
}

const toml::table& DefaultTomlConfig::getKeys() {
        static const toml::table keys = toml::parse(createKeys());

        return keys;
}
