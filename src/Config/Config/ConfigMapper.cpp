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
#include "Applets/Types/AppletRecord.h"
#include "Config/Config/PowerApplet/PowerAppletConfig.h"
#include "Config/Config/Properties/EnvironmentProperties.h"
#include "Config/Config/Properties/LayoutProperties.h"
#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Config/Config/Properties/WindowProperties.h"
#include "Config/Resolvers/Resolvers.h"
#include "Config/TOML/NodeView.h"
#include "Config/TOML/TomlAccessor.h"
#include "Core/Log.h"
#include "UI/Enums/ButtonIDs.h"

#include <algorithm>
#include <cstdlib>
#include <qnamespace.h>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStringList>

class GlobalConfig;

static power_button_id getPowerButtonIDFromString(const QString& string) {
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
void ConfigMapper::mapWindow(NodePair nodes, WindowProperties& window,
                             const WindowProperties& defaults, const QString& path_context) {
        // Resolve power_data and global_data
        auto power_data  = resolve<toml::table>(path_context,
                                                Source{nodes.primary, applet::power_applet.scope});
        auto global_data = resolve<toml::table>(path_context,
                                                Source{nodes.fallback, applet::global.scope});

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) {
                window = defaults;
                return;
        }

        WindowProperties window_properties{};

        window_properties.size =
                resolveOr<QSize>(extendCfgPath(path_context, "size"), defaults.getSize(),
                                 Source{nodes.primary["size"], applet::power_applet.scope},
                                 Source{nodes.fallback["size"], applet::global.scope});

        window_properties.title =
                resolveOr<QString>(extendCfgPath(path_context, "title"), defaults.getTitle(),
                                   Source{nodes.primary["title"], applet::power_applet.scope},
                                   Source{nodes.fallback["title"], applet::power_applet.scope});

        window = std::move(window_properties);
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                    const PrimaryButtonProperties& defaults,
                                    const QString&                 path_context) {
        // Resolve power_data and global_data
        auto power_data  = resolve<toml::table>(path_context,
                                                Source{nodes.primary, applet::power_applet.scope});
        auto global_data = resolve<toml::table>(path_context,
                                                Source{nodes.fallback, applet::global.scope});

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) {
                button = defaults;
                return;
        }

        PrimaryButtonProperties button_properties{};

        button_properties.text_alignment = resolveOr<Qt::Alignment>(
                extendCfgPath(path_context, "text_alignment"), defaults.getTextAlignment(),
                Source{nodes.primary["text_alignment"], applet::power_applet.scope},
                Source{nodes.fallback["text_alignment"], applet::global.scope});

        // TODO This option doesn't work because icon alignment is not applied anywhere yet, fix
        button_properties.icon_alignment = resolveOr<Qt::Alignment>(
                extendCfgPath(path_context, "icon_alignment"), defaults.getIconAlignment(),
                Source{nodes.primary["icon_alignment"], applet::power_applet.scope},
                Source{nodes.fallback["icon_alignment"], applet::global.scope});

        button_properties.icon_size =
                resolveOr<QSize>(extendCfgPath(path_context, "icon_size"), defaults.getIconSize(),
                                 Source{nodes.primary["icon_size"], applet::power_applet.scope},
                                 Source{nodes.fallback["icon_size"], applet::global.scope});

        button_properties.policy =
                resolveOr<QSizePolicy>(extendCfgPath(path_context, "policy"), defaults.getPolicy(),
                                       Source{nodes.primary["policy"], applet::power_applet.scope},
                                       Source{nodes.fallback["policy"], applet::global.scope});

        button = std::move(button_properties);
}

/* Layout Properties */
void ConfigMapper::mapCommandArgument(node_view argument_node, PrimaryButtonData& button,
                                      const PrimaryButtonData& defaults, QStringList& arguments,
                                      size_t button_index, size_t arg_index,
                                      const QString& path_context) {
        QString argument{};

        resolveOrDefault<QString>(path_context, argument, button, defaults,
                                  Source{argument_node, applet::power_applet.scope});
        if (argument.isEmpty()) { return; }

        arguments << std::move(argument);
}

void ConfigMapper::mapCommandArguments(node_view arguments_node, PrimaryButtonData& button,
                                       const PrimaryButtonData& defaults, QStringList& arguments,
                                       size_t button_index, const QString& path_context) {
        constexpr size_t min_size = 0;
        const auto       args = getTomlArray(arguments_node,
                                             makeCfgPath(applet::power_applet.scope, path_context),
                                             "Format: [string, array]", min_size);
        if (!args) {
                button = defaults;
                return;
        }

        QStringList argument_list{};

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapCommandArgument(toml::node_view(args.value()[i]), button, defaults,
                                   argument_list, button_index, i,
                                   makeCfgPath(applet::power_applet.scope, path_context)
                                           + QString("[%1]").arg(i).toStdString().c_str());
        }

        arguments = std::move(argument_list);
}

void ConfigMapper::mapCommand(node_view command_node, PrimaryButtonData& button,
                              const PrimaryButtonData& defaults, ShellCommand& command,
                              size_t button_index, const QString& path_context) {
        QString error_arr_details = "Format: [program, [args...]]";

        constexpr size_t min_size = 2, max_size = 2;
        const auto command_arr = getTomlArray(command_node,
                                              makeCfgPath(applet::power_applet.scope, path_context),
                                              error_arr_details, min_size, max_size);
        if (!command_arr) {
                button = defaults;
                return;
        }

        ShellCommand cmd{};

        resolveOrDefault<QString>(extendCfgPath(path_context, "program"), cmd.program, button,
                                  defaults,
                                  Source{toml::node_view(command_arr.value()[0]),
                                         applet::power_applet.scope});

        mapCommandArguments(toml::node_view(command_arr.value()[1]), button, defaults,
                            cmd.arguments, button_index, extendCfgPath(path_context, "arguments"));

        command = std::move(cmd);
}

bool ConfigMapper::mapPrimaryButton(node_view                             button_data_node,
                                    std::vector<PrimaryButtonData>&       buttons,
                                    const std::vector<PrimaryButtonData>& default_buttons,
                                    const PrimaryButtonData& defaults, size_t button_index,
                                    const QString& path_context) {
        if (button_index > buttons.size()) {
                if (button_index > 1) {
                        QFATAL("The index of button after %s is too high! Index is %zu",
                               buttons[button_index].label, button_index);
                }

                QFATAL("The button index is too high! %zu", button_index);
        }

        const QString button_path_context = path_context + QString("[%1]").arg(button_index);

        const auto button_table = resolve<toml::table>(button_path_context,
                                                       Source{button_data_node,
                                                              applet::power_applet.scope});
        if (!button_table) {
                buttons = default_buttons;
                return true;
        }

        PrimaryButtonData button{};

        auto enabled = resolve<bool>(path_context, Source{button_table.value()["enabled"],
                                                          applet::power_applet.scope});
        if (!enabled) { return true; }

        resolveTransformOrDefault<QString>(extendCfgPath(path_context, "id"), button.id, button,
                                           defaults, getPowerButtonIDFromString,
                                           Source{button_table.value()["id"],
                                                  applet::power_applet.scope});

        resolveOrDefault<QString>(extendCfgPath(path_context, "label"), button.label, button,
                                  defaults,
                                  Source{button_table.value()["label"], applet::power_applet.scope});

        resolveOrDefault<int64_t>(extendCfgPath(path_context, "order"), button.order, button,
                                  defaults,
                                  Source{button_table.value()["order"], applet::power_applet.scope});

        mapCommand(button_data_node["command"], button, defaults, button.command, button_index,
                   extendCfgPath(path_context, "command"));

        buttons.insert(buttons.cend(), std::move(button));

        return false;
}

void ConfigMapper::mapPrimaryButtons(node_view                             primary_buttons_node,
                                     std::vector<PrimaryButtonData>&       primary_buttons,
                                     const std::vector<PrimaryButtonData>& defaults,
                                     const QString&                        path_context) {
        const auto buttons = resolve<toml::array>(path_context, Source{primary_buttons_node,
                                                                       applet::power_applet.scope});
        if (!buttons) {
                primary_buttons = defaults;
                return;
        }

        std::vector<PrimaryButtonData> buttons_found{};

        bool defaulted = false;
        for (size_t i = 0; i != buttons.value().size(); ++i) {
                // TODO Ensure defaults are at least as long as primary_buttons, or pass optional
                //      to avoid adding a button that cannot be defaulted
                defaulted = mapPrimaryButton(node_view(buttons.value().at(i)), buttons_found,
                                             defaults, defaults[i], i,
                                             extendCfgPath(path_context, std::to_string(i).c_str()));
                if (defaulted) { return; }
        }

        if (buttons_found.empty()) {
                QWARNING() << makeCfgPath(applet::power_applet.scope, path_context)
                                      + ", no enabled buttons found!";
                primary_buttons = defaults;
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

void ConfigMapper::mapLayout(node_view layout_node, LayoutProperties& layout,
                             const LayoutProperties& defaults, const QString& path_context) {
        LayoutProperties layout_properties{};

        const auto data = resolve<toml::table>(path_context,
                                               Source{layout_node, applet::power_applet.scope});
        if (!data) {
                layout_properties = defaults;
                layout            = std::move(layout_properties);
                return;
        }

        // Primary power buttons
        mapPrimaryButtons(data.value()["primary_buttons"], layout_properties.primary_power_buttons,
                          defaults.getPrimaryPowerButtons(),
                          extendCfgPath(path_context, "primary_buttons"));

        layout = std::move(layout_properties);
}

void ConfigMapper::mapEnvironment(node_view environment_node, EnvironmentProperties& environment,
                                  const EnvironmentProperties& defaults,
                                  const QString&               path_context) {
        EnvironmentProperties environment_properties{};

        const auto data = resolve<toml::table>(path_context, Source{environment_node,
                                                                    applet::power_applet.scope});
        if (!data) {
                environment_properties = defaults;
                environment            = std::move(environment_properties);
                return;
        }

        // D-Bus mode
        environment_properties.dbus_mode = resolveOr<bool>(extendCfgPath(path_context, "dbus_mode"),
                                                           defaults.getDBusMode(),
                                                           Source{data.value()["dbus_mode"],
                                                                  applet::power_applet.scope});

        environment = std::move(environment_properties);
}

void ConfigMapper::mapToGlobalConfig(const toml::table& config_table, GlobalConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefault();

        if (!config_table.contains(applet::global.scope)) {
                QWARNING() << "in config.toml, global missing!";
        }

        /* Window Properties */
        mapWindow(NodePair{config_table[applet::power_applet.scope]["window"],
                           config_table[applet::global.scope]["window"]},
                  config.window_properties, defaults.getWindowProperties(), "window");

        /* Primary Button Properties */
        mapPrimaryButton(NodePair{config_table[applet::power_applet.scope]["primary_button"],
                                  config_table[applet::global.scope]["primary_button"]},
                         config.primary_button_properties, defaults.getPrimaryButtonProperties(),
                         "primary_button");
}

void ConfigMapper::mapToPowerAppletConfig(const toml::table& config_table,
                                          PowerAppletConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefault();

        if (!config_table.contains(applet::power_applet.scope)) {
                QWARNING() << "in config.toml, power_applet missing!";
        }

        mapToGlobalConfig(config_table, config);

        /* Layout Properties */
        mapLayout(config_table[applet::power_applet.scope]["layout"], config.layout_properties,
                  defaults.getLayoutProperties(), "layout");

        /* Environment Properties */
        mapEnvironment(config_table[applet::power_applet.scope]["environment"],
                       config.environment_properties, defaults.getEnvironmentProperties(),
                       "environment");
}
