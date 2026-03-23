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
#include "Config/Config/Properties/EnvironmentProperties.h"
#include "Config/Config/Properties/LayoutProperties.h"
#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Config/Config/Properties/WindowProperties.h"
#include "Config/TOML/NodeView.h"
#include "Config/TOML/TomlAccessor.h"
#include "Core/Log.h"
#include "CppUtils/include/Enum.h"
#include "UI/Enums/ButtonIDs.h"

#include <algorithm>
#include <cstdlib>
#include <qnamespace.h>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStringList>
#include <QStringLiteral>

using enum_utils::EnumMap;

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
void ConfigMapper::mapWindowSize(node_view size_node, QSize& size) {
        // TODO size_scale config option to let the size be a % of screen size
        // TODO option to automatically detect and assign monitor size to size and then multiply by size_scale
        const auto& defaults = Config::getDefaultConfig().getWindowProperties().getSize();
        size                 = getQSize(size_node, defaults, "in global.window.size");
}

void ConfigMapper::mapWindowTitle(node_view title_node, QString& title) {
        const auto& defaults = Config::getDefaultConfig().getWindowProperties().getTitle();

        title = QString::fromStdString(getOrDefault<std::string>(title_node, defaults.toStdString(),
                                                                 "in global.window.title"));
}

void ConfigMapper::mapWindowProperties(node_view window_node, WindowProperties& window) {
        const auto* data     = getTomlTable(window_node, "in config.toml, global.window");
        const auto& defaults = Config::getDefaultConfig().getWindowProperties();

        if (!data) {
                window = defaults;
                return;
        }

        mapWindowSize((*data)["size"], window.size);
        mapWindowTitle((*data)["title"], window.title);
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButtonTextAlignment(node_view      text_alignment_node,
                                                 Qt::Alignment& text_alignment) {
        const auto& key =
                getOrDefault<std::string>(text_alignment_node, {},
                                          "in config.toml, global.primary_button.text_alignment");
        const auto& defaults =
                Config::getDefaultConfig().getPrimaryButtonProperties().getTextAlignment();

        if (key.empty()) {
                text_alignment = defaults;
                return;
        }

        text_alignment = getAlignment(key, alignment_map, defaults,
                                      "in config.toml, global.primary_button.text_alignment");
}

void ConfigMapper::mapPrimaryButtonIconAlignment(node_view      icon_alignment_node,
                                                 Qt::Alignment& icon_alignment) {
        const auto& key =
                getOrDefault<std::string>(icon_alignment_node, {},
                                          "in config.toml, global.primary_button.icon_alignment");
        const auto& defaults =
                Config::getDefaultConfig().getPrimaryButtonProperties().getIconAlignment();

        if (key.empty()) {
                icon_alignment = defaults;
                return;
        }

        icon_alignment = getAlignment(key, alignment_map, defaults,
                                      "in config.toml, global.primary_button.icon_alignment");
}

void ConfigMapper::mapPrimaryButtonIconSize(node_view icon_size_node, QSize& icon_size) {
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties().getIconSize();

        icon_size = getQSize(icon_size_node, defaults,
                             "in config.toml, global.primary_button.icon_size");
}

void ConfigMapper::mapPrimaryButtonPolicy(node_view policy_node, QSizePolicy& policy) {
        const auto& key      = getOrDefault<std::string>(policy_node, {},
                                                         "in config.toml, global.primary_button.policy");
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties().getPolicy();

        policy = getSizePolicy(key, size_policy_map, defaults,
                               "in config.toml, global.primary_button.policy");
}

void ConfigMapper::mapPrimaryButtonProperties(node_view                button_node,
                                              PrimaryButtonProperties& button) {
        const auto* data     = getTomlTable(button_node, "in config.toml, global.primary_button");
        const auto& defaults = Config::getDefaultConfig().getPrimaryButtonProperties();

        if (!data) {
                button = defaults;
                return;
        }

        // Text alignment
        ConfigMapper::mapPrimaryButtonTextAlignment((*data)["text_alignment"],
                                                    button.text_alignment);

        // TODO This option doesn't work, fix
        // Icon alignment
        ConfigMapper::mapPrimaryButtonIconAlignment((*data)["icon_alignment"],
                                                    button.icon_alignment);

        // Icon size
        ConfigMapper::mapPrimaryButtonIconSize((*data)["icon_size"], button.icon_size);

        // Policy
        ConfigMapper::mapPrimaryButtonPolicy((*data)["policy"], button.policy);
}

/* Layout Properties */
void ConfigMapper::mapLayoutPrimaryButtonID(node_view id_node, PrimaryButtonData& button,
                                            power_button_id& id, size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix = QStringLiteral(
                                       "in config.toml, power_applet.layout.primary_buttons[%1].id")
                                       .arg(button_index);

        auto result = QString::fromStdString(
                getOrDefault<std::string>(id_node, {}, std::move(error_prefix)));

        if (result.isEmpty()) {
                button = defaults;
                return;
        }

        id = getPowerButtonIDFromString(std::move(result));
}

void ConfigMapper::mapLayoutPrimaryButtonLabel(node_view label_node, PrimaryButtonData& button,
                                               QString& label, size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix =
                QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1].label")
                        .arg(button_index);

        auto result = QString::fromStdString(
                getOrDefault<std::string>(label_node, {}, std::move(error_prefix)));

        if (result.isEmpty()) {
                button = defaults;
                return;
        }

        label = result;
}

void ConfigMapper::mapLayoutPrimaryButtonOrder(node_view order_node, PrimaryButtonData& button,
                                               long& order, std::vector<PrimaryButtonData>& buttons,
                                               size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
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
        const long result = getOrDefault<int64_t>(order_node, default_order, std::move(error_prefix));

        if (result != default_order) {
                button = defaults;
                return;
        }

        order = result;
}

void ConfigMapper::mapLayoutPrimaryButtonCommandProgram(node_view          program_node,
                                                        PrimaryButtonData& button, QString& program,
                                                        size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // clang-format off
        QString error_prefix =
                QStringLiteral(
                        "in config.toml, power_applet.layout.primary_buttons[%1].command[0]")
                        .arg(button_index);
        // clang-format on

        program = QString::fromStdString(
                getOrDefault<std::string>(program_node, defaults.command.program.toStdString(),
                                          std::move(error_prefix)));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArgumentsArgument(node_view          argument_node,
                                                                  PrimaryButtonData& button,
                                                                  QStringList&       arguments,
                                                                  size_t             button_index,
                                                                  size_t             arg_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // clang-format off
        const QString error_prefix =
                QStringLiteral(
                        "in config.toml, power_applet.layout.primary_buttons[%1]"
                        ".command[1][%2]").arg(button_index, arg_index);
        // clang-format on

        arguments.insert(arguments.cend(),
                         QString::fromStdString(getOrDefault<std::string>(
                                 argument_node, defaults.command.arguments[arg_index].toStdString(),
                                 error_prefix)));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArguments(node_view          arguments_node,
                                                          PrimaryButtonData& button,
                                                          QStringList&       arguments,
                                                          size_t             button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // clang-format off
        QString error_prefix = QStringLiteral(
                                       "in config.toml, power_applet.layout.primary_buttons[%1]"
                                       ".command[1]").arg(button_index);
        // clang-format on

        constexpr size_t min_size = 0;
        const auto       args     = getTomlArray(arguments_node, min_size, error_prefix,
                                                 "Format: [string, array]");
        if (!args) {
                button = defaults;
                return;
        }

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapLayoutPrimaryButtonCommandArgumentsArgument(toml::node_view(args.value()[i]),
                                                               button, arguments, button_index, i);
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommand(node_view command_node, PrimaryButtonData& button,
                                                 ShellCommand& command, size_t button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_prefix =
                QStringLiteral("in config.toml, power_applet.layout.primary_buttons[%1].command")
                        .arg(button_index);
        QString error_arr_details = "Format: [program, [args...]]";

        constexpr size_t min_size = 2, max_size = 2;
        const auto       command_arr = getTomlArray(command_node, min_size, max_size, error_prefix,
                                                    error_arr_details);
        if (!command_arr) {
                button = defaults;
                return;
        }

        mapLayoutPrimaryButtonCommandProgram(toml::node_view(command_arr.value()[0]), button,
                                             command.program, button_index);

        mapLayoutPrimaryButtonCommandArguments(toml::node_view(command_arr.value()[1]), button,
                                               command.arguments, button_index);
}

void ConfigMapper::mapLayoutPrimaryButtonData(node_view                       button_data_node,
                                              PrimaryButtonData&              button_data,
                                              std::vector<PrimaryButtonData>& buttons,
                                              size_t                          button_index) {
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        mapLayoutPrimaryButtonID(button_data_node["id"], button_data, button_data.id, button_index);

        mapLayoutPrimaryButtonLabel(button_data_node["label"], button_data, button_data.label,
                                    button_index);

        mapLayoutPrimaryButtonOrder(button_data_node["order"], button_data, button_data.order,
                                    buttons, button_index);

        mapLayoutPrimaryButtonCommand(button_data_node["command"], button_data, button_data.command,
                                      button_index);
}

// TODO Extract logic shared with mapLayoutPrimaryButtonData
void ConfigMapper::logButtonDisabled(node_view id_node, PrimaryButtonData& button_data,
                                     size_t button_index) {
        const auto& data     = id_node.as_string();
        const auto& defaults = Config::getDefaultConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) { // TODO Duplication, remove
                QWARNING_NS() << "in config.toml, power_applet.layout.primary_buttons["
                              << button_index << "].id must be a string! Using defaults...";
                button_data = defaults;
                return;
        }

        QDEBUG() << id_node.as_string()->get() << ": DISABLED";
}

void ConfigMapper::mapLayoutPrimaryButtons(node_view                       primary_buttons_node,
                                           std::vector<PrimaryButtonData>& primary_buttons) {
        const auto data = getTomlArray(primary_buttons_node,
                                       "in config.toml, power_applet.layout.primary_buttons");

        const auto& defaults =
                Config::getDefaultConfig().getLayoutProperties().getPrimaryPowerButtons();

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

                const auto* button = getTomlTable(toml::node_view(button_node), error_prefix);
                if (!button) {
                        primary_buttons = defaults;
                        return;
                }

                error_prefix =
                        QStringLiteral(
                                "in config.toml, power_applet.layout.primary_buttons[%1].enabled")
                                .arg(index);

                const bool enabled = getOrDefault((*button)["enabled"], true, error_prefix);
                if (enabled) {
                        mapLayoutPrimaryButtonData(toml::node_view(button), button_data,
                                                   buttons_found, index);

                        buttons_found.push_back(std::move(button_data));
                } else {
                        logButtonDisabled((*button)["id"], button_data, index);
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

        primary_buttons = std::move(buttons_found);
}

void ConfigMapper::mapLayoutProperties(node_view layout_node, LayoutProperties& layout) {
        const auto& defaults = Config::getDefaultConfig().getLayoutProperties();

        const QString error_prefix = "in config.toml, power_applet.layout";

        const auto* data = getTomlTable(layout_node, std::move(error_prefix));
        if (!data) {
                layout = defaults;
                return;
        }

        // Primary power buttons
        ConfigMapper::mapLayoutPrimaryButtons((*data)["primary_buttons"],
                                              layout.primary_power_buttons);
}

void ConfigMapper::mapEnvironmentDBusMode(node_view dbus_mode_node, bool dbus_mode) {
        const auto& defaults = Config::getDefaultConfig().getEnvironmentProperties().getDBusMode();

        const QString error_prefix = "in config.toml, power_applet.environment.dbus_mode";

        dbus_mode = getOrDefault(dbus_mode_node, defaults, std::move(error_prefix));
}

void ConfigMapper::mapEnvironmentProperties(node_view              environment_node,
                                            EnvironmentProperties& environment) {
        const auto& defaults = Config::getDefaultConfig().getEnvironmentProperties();

        const QString error_prefix = "in config.toml, power_applet.environment";

        const auto* data = getTomlTable(environment_node, std::move(error_prefix));
        if (!data) {
                environment = defaults;
                return;
        }

        // D-Bus mode
        ConfigMapper::mapEnvironmentDBusMode((*data)["dbus_mode"], environment.dbus_mode);
}

void ConfigMapper::mapToConfig(const toml::table& config_table, Config& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = Config::getDefaultConfig();

        // Check the validity of global and power_applet
        const QString error_prefix = "in config.toml, global";
        const auto*   global       = getTomlTable(config_table["global"], std::move(error_prefix));
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

        /* Window Properties */
        mapWindowProperties(config_table["global"]["window"], config.window_properties);

        /* Primary Button Properties */
        mapPrimaryButtonProperties(config_table["global"]["primary_button"],
                                   config.primary_button_properties);

        /* Layout Properties */
        mapLayoutProperties(config_table["power_applet"]["layout"], config.layout_properties);

        /* Environment Properties */
        mapEnvironmentProperties(config_table["power_applet"]["environment"],
                                 config.environment_properties);
}
