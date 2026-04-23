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
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/Resolvers/Resolvers.h"
#include "Config/TOML/TomlAccessor.h"
#include "Config/TOML/Types/NodePair.h"
#include "Config/TOML/Types/NodeView.h"
#include "Config/TOML/Types/TomlArrayConditions.h"
#include "Log/Log.h"
#include "UI/Enums/ButtonIDs.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <utility>
#include <vector>
#include <QApplication>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStringList>
#include <Qt>

static power_button_id getPowerButtonIDFromString(const QString& string) {
        static const std::unordered_map<QString, power_button_id> map =
                {{"poweroff", power_button_id::shutdown},
                 {"shutdown", power_button_id::shutdown},
                 {"reboot", power_button_id::reboot},
                 {"suspend", power_button_id::suspend},
                 {"hibernate", power_button_id::hibernate}};

        if (!map.contains(string)) { return power_button_id::none; }

        return map.at(string);
}

template<typename T>
static T mapProperties(NodePair nodes, const T& defaults, const QString& path_context,
                       auto fill_fn) {
        // Resolve power_data and global_data
        constexpr bool is_override_power = true;
        auto power_data = resolve<toml::table>({Source{nodes.primary, applet::power_applet.scope}},
                                               path_context, is_override_power);

        constexpr bool is_override_global = false;
        auto global_data = resolve<toml::table>({Source{nodes.fallback, applet::global.scope}},
                                                path_context, is_override_global);

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) { return defaults; }

        T props{};

        fill_fn(nodes, props, path_context);

        return std::move(props);
}

/* Window Properties */
void ConfigMapper::mapWindow(NodePair nodes, WindowProperties& window,
                             const WindowProperties& defaults, const QString& path_context) {
        window = mapProperties(
                nodes, defaults, path_context,
                [&defaults](NodePair nodes, WindowProperties& window, const QString& path_context) {
                        window.size = resolveOr<QSize>({Source{nodes.primary["size"],
                                                               applet::power_applet.scope},
                                                        Source{nodes.fallback["size"],
                                                               applet::global.scope}},
                                                       defaults.getSize(),
                                                       extendCfgPath(path_context, "size"));

                        window.title = resolveOr<QString>({Source{nodes.primary["title"],
                                                                  applet::power_applet.scope},
                                                           Source{nodes.fallback["title"],
                                                                  applet::power_applet.scope}},
                                                          defaults.getTitle(),
                                                          extendCfgPath(path_context, "title"));
                });
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                    const PrimaryButtonProperties& defaults,
                                    const QString&                 path_context) {
        button = mapProperties(
                nodes, defaults, path_context,
                [&defaults](NodePair nodes, PrimaryButtonProperties& button,
                            const QString& path_context) {
                        button.text_alignment = resolveOr<Qt::Alignment>(
                                {Source{nodes.primary["text_alignment"], applet::power_applet.scope},
                                 Source{nodes.fallback["text_alignment"], applet::global.scope}},
                                defaults.getTextAlignment(),
                                extendCfgPath(path_context, "text_alignment"));

                        // TODO This option doesn't work because icon alignment is not applied anywhere yet, fix
                        button.icon_alignment = resolveOr<Qt::Alignment>(
                                {Source{nodes.primary["icon_alignment"], applet::power_applet.scope},
                                 Source{nodes.fallback["icon_alignment"], applet::global.scope}},
                                defaults.getIconAlignment(),
                                extendCfgPath(path_context, "icon_alignment"));

                        button.icon_size = resolveOr<QSize>({Source{nodes.primary["icon_size"],
                                                                    applet::power_applet.scope},
                                                             Source{nodes.fallback["icon_size"],
                                                                    applet::global.scope}},
                                                            defaults.getIconSize(),
                                                            extendCfgPath(path_context,
                                                                          "icon_size"));

                        button.policy = resolveOr<QSizePolicy>({Source{nodes.primary["policy"],
                                                                       applet::power_applet.scope},
                                                                Source{nodes.fallback["policy"],
                                                                       applet::global.scope}},
                                                               defaults.getPolicy(),
                                                               extendCfgPath(path_context,
                                                                             "policy"));
                });
}

/* Layout Properties */
void ConfigMapper::mapLayout(node_view layout_node, LayoutProperties& layout,
                             const LayoutProperties& defaults, const QString& path_context) {
        constexpr bool   is_override = false;
        LayoutProperties layout_properties{};

        const auto data = resolve<toml::table>({Source{layout_node, applet::power_applet.scope}},
                                               path_context, is_override);
        if (!data) {
                layout_properties = defaults;
                layout            = std::move(layout_properties);
                return;
        }

        // Primary power buttons
        mapPrimaryButtons(data.value()["primary_buttons"], layout_properties.power_buttons,
                          defaults.getPowerButtons(),
                          extendCfgPath(path_context, "primary_buttons"));

        layout = std::move(layout_properties);
}

void ConfigMapper::mapPrimaryButtons(node_view                             primary_buttons_node,
                                     std::vector<PowerButtonParams>&       primary_buttons,
                                     const std::vector<PowerButtonParams>& defaults,
                                     const QString&                        path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size    = 1;

        const auto buttons = resolve<toml::array>({Source{primary_buttons_node,
                                                          applet::power_applet.scope}},
                                                  path_context, is_override,
                                                  {"Format: [primary buttons...]", min_size,
                                                   std::nullopt});
        if (!buttons) {
                primary_buttons = defaults;
                return;
        }

        std::vector<PowerButtonParams> buttons_found{};

        bool defaulted = false;
        for (size_t i = 0; i != buttons.value().size(); ++i) {
                // If index out of bounds for defaults, pass nullptr
                auto* default_button = (i < defaults.size()) ? &defaults[i] : nullptr;
                defaulted =
                        mapPrimaryButton(node_view(buttons.value().at(i)), buttons_found, defaults,
                                         extendCfgPath(path_context,
                                                       QString("[%1]").arg(i).toStdString().c_str(),
                                                       ""));

                if (defaulted) {
                        primary_buttons = std::move(buttons_found);
                        return;
                }
        }

        if (buttons_found.empty()) {
                QWARNING() << makeCfgPath(applet::power_applet.scope, path_context)
                                      + ", no enabled buttons found!";
                primary_buttons = defaults;
                return;
        }

        std::sort(buttons_found.begin(), buttons_found.end(),
                  [](const PowerButtonParams& a, const PowerButtonParams& b) -> bool {
                          return a.order < b.order;
                  });

        // Prevent index out of bound issues and multiple buttons with the same order by
        // re-mapping order to a range from 1 to the total number of buttons
        for (size_t i = 0; i != buttons_found.size(); ++i) { buttons_found[i].order = i + 1; }

        primary_buttons = std::move(buttons_found);
}

bool ConfigMapper::mapPrimaryButton(node_view                             button_params_node,
                                    std::vector<PowerButtonParams>&       buttons,
                                    const std::vector<PowerButtonParams>& defaults,
                                    const QString&                        path_context) {
        constexpr bool is_override = false;

        const auto button_table = resolve<toml::table>({Source{button_params_node,
                                                               applet::power_applet.scope}},
                                                       path_context, is_override);
        if (!button_table) {
                buttons = defaults;
                return true;
        }

        PowerButtonParams button{};

        // TODO Pass .enabled, .id, .label, and .order as part of path context
        auto enabled = resolve<bool>({Source{button_table.value()["enabled"],
                                             applet::power_applet.scope}},
                                     path_context, is_override);
        if (!enabled) { return true; }

        auto id_result = resolve<QString>({Source{button_table.value()["id"],
                                                  applet::power_applet.scope}},
                                          extendCfgPath(path_context, "id"), is_override);
        if (!id_result) {
                buttons = defaults;
                return true;
        } else {
                button.id = getPowerButtonIDFromString(id_result.value());
        }

        auto text_result = resolve<QString>({Source{button_table.value()["text"],
                                                    applet::power_applet.scope}},
                                            extendCfgPath(path_context, "text"), is_override);
        if (!text_result) {
                buttons = defaults;
                return true;
        } else {
                button.text = text_result.value();
        }

        auto order_result = resolve<int64_t>({Source{button_table.value()["order"],
                                                     applet::power_applet.scope}},
                                             extendCfgPath(path_context, "order"), is_override);
        if (!order_result) {
                buttons = defaults;
                return true;
        } else {
                button.order = order_result.value();
        }

        mapCommand(button_params_node["command"], buttons, defaults, button.command,
                   extendCfgPath(path_context, "command"));

        button.icon = iconFor(button.id);

        button.dbus_method = dbusMethodFor(button.id);

        buttons.insert(buttons.cend(), std::move(button));

        return false;
}

void ConfigMapper::mapCommand(node_view command_node, std::vector<PowerButtonParams>& buttons,
                              const std::vector<PowerButtonParams>& defaults, ShellCommand& command,
                              const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size = 2, max_size = 2;
        const auto command_arr = resolve<toml::array>({Source{command_node,
                                                              applet::power_applet.scope}},
                                                      path_context, is_override,
                                                      {"Format: [program, [args...]]", min_size,
                                                       max_size});
        if (!command_arr) {
                buttons = defaults;
                return;
        }

        ShellCommand cmd{};

        auto program_result = resolve<QString>({Source{toml::node_view(command_arr.value()[0]),
                                                       applet::power_applet.scope}},
                                               extendCfgPath(path_context, "program"), is_override);
        if (!program_result) {
                buttons = defaults;
                return;
        } else {
                cmd.program = program_result.value();
        }

        mapCommandArguments(toml::node_view(command_arr.value()[1]), buttons, defaults,
                            cmd.arguments, extendCfgPath(path_context, "arguments"));

        command = std::move(cmd);
}

void ConfigMapper::mapCommandArguments(node_view                             arguments_node,
                                       std::vector<PowerButtonParams>&       buttons,
                                       const std::vector<PowerButtonParams>& defaults,
                                       QStringList& arguments, const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size    = 0;
        const auto args = resolve<toml::array>({Source{arguments_node, applet::power_applet.scope}},
                                               path_context, is_override,
                                               {"Format: [string, array]", min_size, std::nullopt});
        if (!args) {
                buttons = defaults;
                return;
        }

        QStringList argument_list{};

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapCommandArgument(toml::node_view(args.value()[i]), buttons, defaults,
                                   argument_list,
                                   path_context + QString("[%1]").arg(i).toStdString().c_str());
        }

        arguments = std::move(argument_list);
}

void ConfigMapper::mapCommandArgument(node_view                             argument_node,
                                      std::vector<PowerButtonParams>&       buttons,
                                      const std::vector<PowerButtonParams>& defaults,
                                      QStringList& arguments, const QString& path_context) {
        QString        argument{};
        constexpr bool is_override = false;

        auto argument_result = resolve<QString>({Source{argument_node, applet::power_applet.scope}},
                                                path_context, is_override);
        if (!argument_result) {
                buttons = defaults;
                return;
        } else {
                argument = argument_result.value();
        }

        if (argument.isEmpty()) { return; }

        arguments << std::move(argument);
}

void ConfigMapper::mapEnvironment(node_view environment_node, EnvironmentProperties& environment,
                                  const EnvironmentProperties& defaults,
                                  const QString&               path_context) {
        constexpr bool        is_override = false;
        EnvironmentProperties environment_properties{};

        const auto data = resolve<toml::table>({Source{environment_node,
                                                       applet::power_applet.scope}},
                                               path_context, is_override);
        if (!data) {
                environment_properties = defaults;
                environment            = std::move(environment_properties);
                return;
        }

        // D-Bus mode
        environment_properties.dbus_mode =
                resolveOr<bool, bool>({Source{data.value()["dbus_mode"],
                                              applet::power_applet.scope}},
                                      defaults.getDBusMode(),
                                      extendCfgPath(path_context, "dbus_mode"));

        environment = std::move(environment_properties);
}
