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
#include "Config/TOML/TomlAccessor.h"
#include "Core/Log.h"
#include "CppUtils/include/Enum.h"
#include "UI/Enums/ButtonIDs.h"

#include <algorithm>
#include <cstdlib>
#include <qnamespace.h>
#include <string>
#include <toml++/impl/value.hpp>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStringList>
#include <QStringLiteral>

using enum_utils::EnumMap;

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

Qt::Alignment getAlignment(const std::string key, const EnumMap<Qt::Alignment>& map,
                           const Qt::Alignment& fallback, const QString& error_prefix) {
        return getValueFromEnumMap<Qt::Alignment>(key, map, fallback, error_prefix);
}

QSizePolicy getSizePolicy(const std::string key, const EnumMap<QSizePolicy>& map,
                          const QSizePolicy& fallback, const QString& error_prefix) {
        return getValueFromEnumMap<QSizePolicy>(key, map, fallback, error_prefix);
}

/* Window Properties */
void ConfigMapper::mapWindowSize(const toml::table& window, Config& config) {
        // TODO size_scale config option to let the size be a % of screen size
        // TODO option to automatically detect and assign monitor size to size and then multiply by size_scale
        const auto& defaults          = Config::getDefaultConfig().getWindowProperties().getSize();
        config.window_properties.size = getQSize(window["size"], defaults, "in global.window.size");
}

void ConfigMapper::mapWindowTitle(const toml::table& window, Config& config) {
        const auto& defaults = Config::getDefaultConfig().getWindowProperties().getTitle();

        config.window_properties.title = QString::fromStdString(
                getOrDefault<std::string>(window["title"], defaults.toStdString(),
                                          "in global.window.title"));
}

void ConfigMapper::mapWindowProperties(const toml::table& config_table, Config& config) {
        const auto* data     = getTable(config_table["global"]["window"],
                                        "in config.toml, global.window");
        const auto& defaults = Config::getDefaultConfig().getWindowProperties();

        if (!data) {
                config.window_properties = defaults;
                return;
        }

        mapWindowSize(*data, config);
        mapWindowTitle(*data, config);
}

/* Button Properties*/
void ConfigMapper::mapButtonTextAlignment(const toml::table& button, Config& config) {
        const auto& key =
                getOrDefault<std::string>(button["text_alignment"], {},
                                          "in config.toml, global.primary_button.text_alignment");
        const auto& defaults =
                Config::getDefaultConfig().getPrimaryButtonProperties().getTextAlignment();

        if (key.empty()) {
                config.primary_button_properties.text_alignment = defaults;
                return;
        }

        config.primary_button_properties.text_alignment =
                getAlignment(key, alignment_map, defaults,
                             "in config.toml, global.primary_button.text_alignment");
}

void ConfigMapper::mapButtonIconAlignment(const toml::table& button, Config& config) {
        const auto& key =
                getOrDefault<std::string>(button["icon_alignment"], {},
                                          "in config.toml, global.primary_button.icon_alignment");
        const auto& defaults =
                Config::getDefaultConfig().getPrimaryButtonProperties().getIconAlignment();

        if (key.empty()) {
                config.primary_button_properties.icon_alignment = defaults;
                return;
        }

        config.primary_button_properties.icon_alignment =
                getAlignment(key, alignment_map, defaults,
                             "in config.toml, global.primary_button.icon_alignment");
}

void ConfigMapper::mapButtonIconSize(const toml::table& button, Config& config) {
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties().getIconSize();

        config.primary_button_properties.icon_size =
                getQSize(button["icon_size"], defaults,
                         "in config.toml, global.primary_button.icon_size");
}

void ConfigMapper::mapButtonPolicy(const toml::table& button, Config& config) {
        const auto& key      = getOrDefault<std::string>((button)["policy"], {},
                                                         "in config.toml, global.primary_button.policy");
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties().getPolicy();

        config.primary_button_properties.policy =
                getSizePolicy(key, size_policy_map, defaults,
                              "in config.toml, global.primary_button.policy");
}

void ConfigMapper::mapButtonProperties(const toml::table& config_table, Config& config) {
        const auto* data     = getTable(config_table["global"]["primary_button"],
                                        "in config.toml, global.primary_button");
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties();

        if (!data) {
                config.primary_button_properties = defaults;
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
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix =
                QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1].identifier")
                        .arg(button_index);

        auto result = QString::fromStdString(
                getOrDefault<std::string>(node, {}, std::move(error_prefix)));

        if (!result.isEmpty()) {
                button = defaults;
                return;
        }

        identifier = getPowerButtonIDFromString(std::move(result));
}

void ConfigMapper::mapLayoutPrimaryButtonText(const toml::node_view<const toml::node> node,
                                              PrimaryButtonData& button, QString& text,
                                              size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix =
                QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1].text")
                        .arg(button_index);

        auto result = QString::fromStdString(
                getOrDefault<std::string>(node, {}, std::move(error_prefix)));

        if (!result.isEmpty()) {
                button = defaults;
                return;
        }

        text = result;
}

void ConfigMapper::mapLayoutPrimaryButtonOrder(const toml::node_view<const toml::node> node,
                                               PrimaryButtonData&                      button,
                                               std::vector<PrimaryButtonData>& buttons, long& order,
                                               size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix =
                QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1].order")
                        .arg(button_index);

        // Workaround that creates a unique number corresponding to a default from the largest
        // order in all primary power buttons + 1
        // It will overflow if some of the buttons define the largest order possible for a long
        // TODO Eliminate the button order workaround
        long default_order = 0;
        for (const auto& button_data : buttons) {
                default_order = std::max(default_order, button_data.order + 1);
        }
        const long result = getOrDefault<int64_t>(node, 0, std::move(error_prefix));

        if (result != default_order) {
                button = defaults;
                return;
        }

        order = result;
}

void ConfigMapper::mapLayoutPrimaryButtonCommandProgram(const toml::node_view<const toml::node> node,
                                                        PrimaryButtonData& button, QString& program,
                                                        size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // clang-format off
        QString error_prefix =
                QStringLiteral(
                        "in config.toml, power_applet.layout.primary_buttons[%1].command[0]")
                        .arg(button_index);
        // clang-format on

        program = QString::fromStdString(
                getOrDefault<std::string>(node, defaults.command.program.toStdString(),
                                          std::move(error_prefix)));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArgumentsArgument(
        const toml::node_view<const toml::node> node, PrimaryButtonData& button,
        QStringList& arguments, size_t button_index, size_t arg_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // clang-format off
        const QString error_prefix =
                QStringLiteral(
                        "in config.toml, power_applet.layout.primary_buttons[%1]"
                        ".command[1][%2]").arg(button_index, arg_index);
        // clang-format on

        arguments.insert(arguments.cend(),
                         QString::fromStdString(getOrDefault<std::string>(
                                 node, defaults.command.arguments[arg_index].toStdString(),
                                 error_prefix)));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArguments(
        const toml::node_view<const toml::node> node, PrimaryButtonData& button,
        QStringList& arguments, size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // clang-format off
        QString error_prefix = QStringLiteral(
                                       "in config.toml, power_applet.layout.primary_buttons[%1]"
                                       ".command[1]").arg(button_index);
        // clang-format on

        constexpr size_t min_size = 0;
        const auto args = getTomlArray(node, min_size, error_prefix, "Format: [string, array]");
        if (!args) {
                button = defaults;
                return;
        }

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapLayoutPrimaryButtonCommandArgumentsArgument(toml::node_view(args.value()[i]),
                                                               button, arguments, button_index, i);
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommand(const toml::node_view<const toml::node> node,
                                                 PrimaryButtonData& button, size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix =
                QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1].command")
                        .arg(button_index);
        QString error_arr_details = "Format: [program, [args...]]";

        constexpr size_t min_size = 2, max_size = 2;
        const auto command = getTomlArray(node, min_size, max_size, error_prefix, error_arr_details);
        if (!command) {
                button = defaults;
                return;
        }

        mapLayoutPrimaryButtonCommandProgram(toml::node_view(command.value()[0]), button,
                                             button.command.program, button_index);

        mapLayoutPrimaryButtonCommandArguments(toml::node_view(command.value()[1]), button,
                                               button.command.arguments, button_index);
}

void ConfigMapper::mapLayoutPrimaryButtonData(const toml::table&              button_table,
                                              PrimaryButtonData&              button_data,
                                              std::vector<PrimaryButtonData>& buttons,
                                              size_t                          button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getWindowLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        mapLayoutPrimaryButtonIdentifier(button_table["id"], button_data, button_data.identifier,
                                         button_index);

        mapLayoutPrimaryButtonText(button_table["label"], button_data, button_data.text,
                                   button_index);

        mapLayoutPrimaryButtonOrder(button_table["order"], button_data, buttons, button_data.order,
                                    button_index);

        mapLayoutPrimaryButtonCommand(button_table["command"], button_data, button_index);
}

// TODO Extract logic shared with mapLayoutPrimaryButtonData
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
        // clang-format off
        const auto data = getTomlArray(
                layout["primary_buttons"],
                "in config.toml, power_applet.layout.primary_buttons");
        // clang-format on

        const auto& defaults =
                Config::getDefaultConfig().getWindowLayoutProperties().getPrimaryPowerButtons();

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

                QString error_prefix =
                        QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1] ")
                                .arg(index);

                const auto* button = getTable(toml::node_view(button_node), error_prefix);
                if (!button) {
                        config.window_layout_properties = defaults;
                        return;
                }

                error_prefix =
                        QStringLiteral(
                                "in config.toml, power_applet.layout.primary_buttons[%1].enabled")
                                .arg(index);

                const bool enabled = getOrDefault((*button)["enabled"], true, error_prefix);
                if (enabled) {
                        mapLayoutPrimaryButtonData(*button, button_data, buttons_found, index);

                        buttons_found.push_back(std::move(button_data));
                } else {
                        logButtonDisabled(*button, button_data, index);
                }
        }

        if (buttons_found.empty()) {
                QWARNING() << "in config.toml, no enabled buttons found!";
                return;
        }

        std::sort(buttons_found.begin(), buttons_found.end(),
                  [](const PrimaryButtonData& a, const PrimaryButtonData& b) -> bool {
                          return a.order < b.order;
                  });

        // Prevent index out of bound issues and multiple buttons with the same order by
        // re-mapping order to a range from 1 to the total number of buttons
        for (size_t i = 0; i != buttons_found.size(); ++i) { buttons_found[i].order = i + 1; }

        config.window_layout_properties.primary_power_buttons = std::move(buttons_found);
}

void ConfigMapper::mapLayoutProperties(const toml::table& config_table, Config& config) {
        const auto& defaults = config.getWindowLayoutProperties();

        const QString error_prefix = "in config.toml, power_applet.layout";

        const auto* data = getTable(config_table["power_applet"]["layout"], std::move(error_prefix));
        if (!data) {
                config.window_layout_properties = defaults;
                return;
        }

        // Primary power buttons
        ConfigMapper::mapLayoutPrimaryButtons(*data, config);
}

void ConfigMapper::mapEnvironmentDBusMode(const toml::table& environment, Config& config) {
        const auto& defaults = Config::getDefaultConfig().getEnvironmentProperties().getDBusMode();

        const QString error_prefix = "in config.toml, power_applet.environment.dbus_mode";

        config.environment_properties.dbus_mode = getOrDefault(environment["dbus_mode"], defaults,
                                                               std::move(error_prefix));
}

void ConfigMapper::mapEnvironmentProperties(const toml::table& config_table, Config& config) {
        const auto& defaults = Config::getDefaultConfig().getEnvironmentProperties();

        const QString error_prefix = "in config.toml, power_applet.environment";

        const auto* data = getTable(config_table["power_applet"]["environment"],
                                    std::move(error_prefix));
        if (!data) {
                config.environment_properties = defaults;
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
        const QString error_prefix = "in config.toml, global";
        const auto*   global       = getTable(config_table["global"], std::move(error_prefix));
        if (!global) {
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
