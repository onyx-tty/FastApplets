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

#include "ConfigMapper.h"
#include "Config.h"
#include "Core/Log.h"
#include "CppUtils/include/Enum.h"
#include "CppUtils/include/String.h"
#include "TOML/TomlParser.h"
#include "UI/Enums/ButtonIDs.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <qnamespace.h>
#include <string>
#include <toml++/impl/value.hpp>
#include <toml++/toml.hpp>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStringList>

using enum_utils::EnumMap;
using string_utils::toLowerCopy;

// TODO Make function names consistent, i.e. mapButtonProperties -> mapPrimaryButtonProperties

namespace {

const EnumMap<Qt::Alignment> alignment_map = {{"top", Qt::AlignTop | Qt::AlignHCenter},
                                              {"center", Qt::AlignCenter},
                                              {"bottom", Qt::AlignBottom | Qt::AlignHCenter},
                                              {"left", Qt::AlignVCenter | Qt::AlignLeft},
                                              {"right", Qt::AlignVCenter | Qt::AlignRight}};

const EnumMap<QSizePolicy> size_policy_map = {{"expanding",
                                               {QSizePolicy::Expanding, QSizePolicy::Expanding}},
                                              {"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}}};

} // namespace

power_button_id getPowerButtonIDFromString(const QString& string) {
        const std::unordered_map<QString, power_button_id> map =
                {{"poweroff", power_button_id::shutdown},
                 {"shutdown", power_button_id::shutdown},
                 {"reboot", power_button_id::reboot},
                 {"suspend", power_button_id::suspend},
                 {"hibernate", power_button_id::hibernate}};

        if (!map.contains(string)) { return power_button_id::none; }

        return map.at(string);
}

/* Window Properties */
void ConfigMapper::mapWindowSize(const toml::table& window, Config& config) {
        // TODO size_scale config option to let the size be a % of screen size
        // TODO option to automatically detect and assign monitor size to size and then multiply by size_scale
        const auto& data     = (window)["size"].as_array();
        const auto& defaults = Config::getDefaultConfig().getWindowProperties().getSize();
        auto&       size     = config.window_properties.size;

        if (!data) {
                QWARNING() << "in config.toml, global.window.size must be an array!"
                           << "Using defaults...";
                size = defaults;
                return;
        } else {
                constexpr size_t            arr_length = 2;
                std::array<int, arr_length> for_qsize{};

                if (arr_length > data->size()) {
                        QWARNING_NS() << "in config.toml, global.window.size must have["
                                      << arr_length << "] integers! Using defaults...";
                        size = defaults;
                        return;
                }

                for (size_t i = 0; i != std::min(data->size(), arr_length); ++i) {
                        const auto& index = data->get(i)->as_integer();

                        if (!index) {
                                const auto& str_index = data->get(i)->as_string();

                                QWARNING_NS() << "in config.toml, global.window.size[" << i
                                              << "] must be an integer! Attempting conversion from"
                                              << "string to integer...";
                                if (!str_index) {
                                        QWARNING_NS() << "FAILED CONVERSION! Using defaults...";
                                        size = defaults;
                                        return;
                                } else {
                                        for_qsize[i] = std::stoi(str_index->get());
                                        QINFO() << "Successful conversion!";
                                }
                        } else {
                                for_qsize[i] = index->get();
                        }
                }

                size = QSize(for_qsize[0], for_qsize[1]);
        }
}

void ConfigMapper::mapWindowTitle(const toml::table& window, Config& config) {
        // TODO Convert to string
        const auto& data     = (window)["title"].as_string();
        const auto& defaults = Config::getDefaultConfig().getWindowProperties().getTitle();
        auto&       title    = config.window_properties.title;

        if (!data) {
                QDEBUG() << "in config.toml, global.window.title must be a string!"
                         << "Using defaults...";
                title = defaults;
                return;
        }

        title = QString::fromStdString(data->get());
}

// TODO Detect mismatched types, log them. Example: "expected int but got string"
// TODO Apply toLowerCopy where applicable
void ConfigMapper::mapWindowProperties(const toml::table& config_table, Config& config) {
        const auto& data     = config_table["global"]["window"].as_table();
        const auto& defaults = Config::getDefaultConfig().getWindowProperties();
        auto&       window   = config.window_properties;

        if (!data) {
                QWARNING() << "global.window must be a table! Using defaults...";
                window = defaults;
                return;
        }

        // Window size
        ConfigMapper::mapWindowSize(*data, config);

        // Window title
        ConfigMapper::mapWindowTitle(*data, config);
}

/* Button Properties*/
void ConfigMapper::mapButtonTextAlignment(const toml::table& button, Config& config) {
        const auto& data = (button)["text_alignment"].as_string();
        const auto& defaults =
                Config::getDefaultConfig().getPrimaryButtonProperties().getTextAlignment();
        auto& text_alignment = config.primary_button_properties.text_alignment;

        if (!data) {
                QWARNING() << "in config.toml, global.primary_button.text_alignment"
                           << "must be a string! Using defaults...";
                text_alignment = defaults;
                return;
        } else {
                if (!alignment_map.contains(data->get())) {
                        QWARNING() << "in config.toml, global.primary_button.text_alignment is"
                                   << "invalid! Using defaults...";
                        text_alignment = defaults;
                        return;
                }

                text_alignment = alignment_map.at(toLowerCopy(data->get()));
        }
}

void ConfigMapper::mapButtonIconAlignment(const toml::table& button, Config& config) {
        const auto& data = (button)["icon_alignment"].as_string();
        const auto& defaults =
                Config::getDefaultConfig().getPrimaryButtonProperties().getIconAlignment();
        auto& icon_alignment = config.primary_button_properties.icon_alignment;

        if (!data) {
                QWARNING() << "in config.toml, global.primary_button.icon_alignment"
                           << "must be a string! Using defaults...";
                icon_alignment = defaults;
                return;
        } else {
                if (!alignment_map.contains(data->get())) {
                        QWARNING() << "in config.toml, global.primary_button.icon_alignment is"
                                   << "invalid! Using defaults...";
                        icon_alignment = defaults;
                        return;
                }

                icon_alignment = alignment_map.at(toLowerCopy(data->get()));
        }
}

void ConfigMapper::mapButtonIconSize(const toml::table& button, Config& config) {
        const auto& data = (button)["icon_size"].as_array();
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties().getIconSize();
        auto& icon_size = config.primary_button_properties.icon_size;

        if (!data) {
                // TODO If icon_size is int, apply to both. If not, try to convert. If fails, default.
                QWARNING() << "in config.toml, global.primary_button.icon_size must be an array!"
                           << "Using defaults...";
                icon_size = defaults;
                return;
        } else {
                constexpr size_t            arr_length = 2;
                std::array<int, arr_length> for_qsize{};

                if (arr_length > data->size()) {
                        // TODO If icon_size[0] exists and is int, apply to both. If not, try to convert. If fails, default.
                        QWARNING() << "in config.toml, global.primary_button.icon_size must have"
                                   << arr_length << "integers! Using defaults...";
                        icon_size = defaults;
                        return;
                }

                for (size_t i = 0; i != std::min(data->size(), arr_length); ++i) {
                        // TODO validate data->get(i) before accessing it as integer
                        const auto& index = data->get(i)->as_integer();
                        if (!index) {
                                // TODO If icon_size[0] exists and is int, apply to both. If not, try to convert. If fails, default.
                                QWARNING_NS() << "in config.toml, global.primary_button.icon_size["
                                              << i << "] must be an integer! Using defaults...";
                                icon_size = defaults;
                                return;
                        }

                        for_qsize[i] = index->get();
                }

                icon_size = QSize(for_qsize[0], for_qsize[1]);
        }
}

void ConfigMapper::mapButtonPolicy(const toml::table& button, Config& config) {
        const auto& data     = (button)["policy"].as_string();
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties().getPolicy();
        auto&       policy   = config.primary_button_properties.policy;

        if (!data) {
                QWARNING() << "in config.toml, global.primary_button.policy must be a string!"
                           << "Using defaults...";
                policy = defaults;

        } else {
                if (!size_policy_map.contains(data->get())) {
                        QWARNING() << "in config.toml, global.primary_button.button_policy is"
                                   << "invalid! Using defaults...";
                        policy = defaults;
                        return;
                }

                policy = size_policy_map.at(toLowerCopy(data->get()));
        }
}

void ConfigMapper::mapButtonProperties(const toml::table& config_table, Config& config) {
        const auto& data           = config_table["global"]["primary_button"].as_table();
        const auto& defaults       = Config::getDefaultConfig().getPrimaryButtonProperties();
        auto&       primary_button = config.primary_button_properties;

        if (!data) {
                QWARNING() << "in config.toml, global.primary_button must be a table!"
                           << "Using defaults...";
                primary_button = defaults;
                return;
        }

        // Text alignment
        ConfigMapper::mapButtonTextAlignment(*data, config);

        // TODO This option doesn't work, fix
        // Icon alignment
        ConfigMapper::mapButtonIconAlignment(*data, config);

        // Icon size
        ConfigMapper::mapButtonIconSize(*data, config);

        // Policy
        ConfigMapper::mapButtonPolicy(*data, config);
}

/* Layout Properties */
void ConfigMapper::mapLayoutPrimaryButtonIdentifier(const toml::node_view<const toml::node> node,
                                                    PrimaryButtonData&                      button,
                                                    power_button_id& identifier,
                                                    size_t           button_index) {
        const auto* data     = node.as_string();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) {
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].id must be a string! Using defaults...";
                button = defaults;
                return;
        } else {
                identifier = getPowerButtonIDFromString(
                        QString::fromStdString(toLowerCopy(data->get())));
        }
}

void ConfigMapper::mapLayoutPrimaryButtonText(const toml::node_view<const toml::node> node,
                                              PrimaryButtonData& button, QString& text,
                                              size_t button_index) {
        const auto* data     = node.as_string();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) {
                // TODO Attempt conversion. If fails, default.
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].label must be a string! Using defaults...";
                button = defaults;
                return;
        } else {
                text = QString::fromStdString(data->get());
        }
}

void ConfigMapper::mapLayoutPrimaryButtonOrder(const toml::node_view<const toml::node> node,
                                               PrimaryButtonData& button, long& order,
                                               size_t button_index) {
        const auto* data     = node.as_integer();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) {
                // TODO If order exists and is not int, try to convert. If fails, default.
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].order must be an integer! Using defaults...";
                button = defaults;
                return;
        } else {
                order = data->get();
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommandProgram(const toml::node_view<const toml::node> node,
                                                        PrimaryButtonData& button, QString& program,
                                                        size_t button_index) {
        const auto* data     = node.as_string();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) {
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].command[0] must be a string! "
                              << "Using defaults...";
                button = defaults;
                return;
        } else {
                program = QString::fromStdString(data->get());
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArgumentsArgument(
        const toml::node_view<const toml::node> node, PrimaryButtonData& button,
        QStringList& arguments, size_t button_index, size_t arg_index) {
        const auto* data     = node.as_string();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) {
                QWARNING_NS() << "in config.toml, power_applet"
                              << ".layout.primary_buttons[" << button_index << "].command[1]["
                              << arg_index << "] must be a string! "
                              << "Using defaults...";
                button = defaults;
                return;
        } else {
                arguments.insert(arguments.cend(), QString::fromStdString(data->get()));
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArguments(
        const toml::node_view<const toml::node> node, PrimaryButtonData& button,
        QStringList& arguments, size_t button_index) {
        const auto* data     = node.as_array();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) {
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].command[1] must be an array! "
                              << "Using defaults...";
                button = defaults;
                return;
        } else {
                const auto& args = *data;
                if (args.empty()) {
                        QWARNING_NS() << "in config.toml, power_applet.layout"
                                      << ".primary_buttons[" << button_index
                                      << "].command[1] must not be empty! "
                                      << "Using defaults...";
                        button = defaults;
                        return;
                }
                for (size_t i = 0; i != args.size(); ++i) {
                        mapLayoutPrimaryButtonCommandArgumentsArgument(toml::node_view(args[i]),
                                                                       button, arguments,
                                                                       button_index, i);
                }
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommand(const toml::node_view<const toml::node> node,
                                                 PrimaryButtonData& button, size_t button_index) {
        const auto* data     = node.as_array();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        ShellCommand shell_command{};

        if (!data) {
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].command must be an array! Using defaults...";
                button = defaults;
                return;
        } else {
                const auto& data_arr = *data;

                if (!(data_arr.size() > 0) && !(data_arr.size() <= 2)) {
                        QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                                      << button_index
                                      << "].command requires 'program' and 'args' elements! "
                                      << "Using defaults...";
                        button = defaults;
                        return;
                } else {
                        mapLayoutPrimaryButtonCommandProgram(toml::node_view(data_arr[0]), button,
                                                             shell_command.program, button_index);

                        mapLayoutPrimaryButtonCommandArguments(toml::node_view(data_arr[1]), button,
                                                               shell_command.arguments,
                                                               button_index);
                }
        }

        button.command = std::move(shell_command);
}

void ConfigMapper::mapLayoutPrimaryButtonData(const toml::table& button_table,
                                              PrimaryButtonData& button_data, size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        mapLayoutPrimaryButtonIdentifier(button_table["id"], button_data,
                                         button_data.identifier, button_index);

        mapLayoutPrimaryButtonText(button_table["label"], button_data, button_data.text,
                                   button_index);

        mapLayoutPrimaryButtonOrder(button_table["order"], button_data, button_data.order,
                                    button_index);

        mapLayoutPrimaryButtonCommand(button_table["command"], button_data, button_index);
}

void ConfigMapper::logButtonDisabled(const toml::table& button_table,
                                     PrimaryButtonData& button_data, size_t button_index) {
        const auto& data     = (button_table)["id"].as_string();
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) { // TODO Duplication, remove
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].id must be a string! Using defaults...";
                button_data = defaults;
                return;
        }

        QDEBUG() << (button_table)["id"].as_string()->get() << ": DISABLED";
}

void ConfigMapper::mapLayoutPrimaryButtons(const toml::table& layout, Config& config) {
        const auto& data = (layout)["primary_buttons"].as_array();
        const auto& defaults =
                Config::getDefaultConfig().getWindowLayoutProperties().getPrimaryPowerButtons();
        auto& primary_buttons = config.window_layout_properties.primary_power_buttons;

        if (!data) {
                QWARNING() << "in config.toml, power_applet.layout.primary_buttons"
                           << "must be a table! Using defaults...";
                primary_buttons = defaults;
                return;
        }

        std::vector<PrimaryButtonData> buttons_found{};

        // TODO Split and simplify
        for (auto& button_node : *data) {
                // Find position of node in primary_buttons
                // TODO Simplify, find_if I think is redundant
                const auto& it = std::find_if(data->begin(), data->end(),
                                              [&button_node](const toml::node& n) -> bool {
                                                      return &n == &button_node;
                                              });

                const size_t index = std::distance(data->begin(), it);

                PrimaryButtonData button_data{};

                const auto& button = button_node.as_table();
                if (!button) {
                        QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                                      << index << "] must be a table! Using defaults...";
                        primary_buttons = defaults;
                        return;
                }

                const auto& enabled_opt = (*button)["enabled"].as_boolean();
                if (!enabled_opt) {
                        // TODO Attempt conversion. If fails, default.
                        QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                                      << index << "].enabled must be a boolean! Using defaults...";
                        primary_buttons = defaults;
                        return;
                } else {
                        bool enabled = enabled_opt->value_or(true);
                        if (enabled) {
                                mapLayoutPrimaryButtonData(*button, button_data, index);

                                buttons_found.push_back(std::move(button_data));
                        } else {
                                logButtonDisabled(*button, button_data, index);
                        }
                }
        }

        if (buttons_found.empty()) {
                QWARNING() << "No enabled buttons found in config.toml!";
                return;
        }

        // TODO Handle multiple order integers of the same value
        std::sort(buttons_found.begin(), buttons_found.end(),
                  [](const PrimaryButtonData& a, const PrimaryButtonData& b) -> bool {
                          return a.order < b.order;
                  });

        // Re-map order to a range of 1 to the number of buttons
        for (size_t i = 0; i != buttons_found.size(); ++i) { buttons_found[i].order = i + 1; }

        primary_buttons = std::move(buttons_found);
}

void ConfigMapper::mapLayoutProperties(const toml::table& config_table, Config& config) {
        const auto& data          = config_table["power_applet"]["layout"].as_table();
        const auto& defaults      = config.getWindowLayoutProperties();
        auto&       window_layout = config.window_layout_properties;

        if (!data) {
                QWARNING() << "in config.toml, power_applet.layout must be a table!"
                           << "Using defaults...";
                window_layout = defaults;
                return;
        }

        // Primary power buttons
        ConfigMapper::mapLayoutPrimaryButtons(*data, config);
}

void ConfigMapper::mapEnvironmentDBusMode(const toml::table& environment, Config& config) {
        const auto& data      = (environment)["dbus_mode"].as_boolean();
        const auto& defaults  = Config::getDefaultConfig().getEnvironmentProperties().getDBusMode();
        auto&       dbus_mode = config.environment_properties.dbus_mode;

        if (!data) {
                QWARNING() << "in config.toml, power_applet.environment.dbus_mode"
                           << "must be a boolean! Using defaults...";
                dbus_mode = defaults;
                return;
        }

        dbus_mode = data;
}

void ConfigMapper::mapEnvironmentProperties(const toml::table& config_table, Config& config) {
        const auto& data        = config_table["power_applet"]["environment"].as_table();
        const auto& defaults    = Config::getDefaultConfig().getEnvironmentProperties();
        auto&       environment = config.environment_properties;

        if (!data) {
                QWARNING() << "in config.toml, power_applet.environment must be a table!"
                           << "Using defaults...";
                environment = defaults;
                return;
        }

        // D-Bus mode
        ConfigMapper::mapEnvironmentDBusMode(*data, config);
}

void ConfigMapper::mapToConfig(const toml::table& config_table, Config& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = Config::getDefaultConfig();

        // Check the validity of global and power_applet
        const auto& global = config_table["global"].as_table();
        if (!global) {
                QWARNING() << "in config.toml, global must be a table! Using defaults...";
                config = defaults;
                return;
        }

        // NOT IMPLEMENTED YET
        /*
        const auto power_applet = config_table["power_applet"].as_table();
        // TODO Defaults
        if (!power_applet) {
                QFATAL("in config.toml, power_applet must be a table! Using defaults...");
        }
        */

        /* Window properties */
        mapWindowProperties(config_table, config);

        /* Button properties */
        mapButtonProperties(config_table, config);

        /* Window layout properties */
        mapLayoutProperties(config_table, config);

        /* Environment Properties */
        mapEnvironmentProperties(config_table, config);
}
