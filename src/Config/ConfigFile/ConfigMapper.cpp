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
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
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
#include <qnamespace.h>
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

class GlobalConfig;

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

static QIcon iconFor(power_button_id id) {
        Q_INIT_RESOURCE(Icons);

        switch (id) {
        case power_button_id::shutdown:  return QIcon{":/Icons/Power/shutdown.svg"};
        case power_button_id::reboot:    return QIcon{":/Icons/Power/reboot.svg"};
        case power_button_id::suspend:   return QIcon{":/Icons/Power/suspend.svg"};
        case power_button_id::hibernate: return QIcon{":/Icons/Power/hibernate.svg"};
        }

        return {};
}

static QString dbusMethodFor(power_button_id id) {
        switch (id) {
        case power_button_id::shutdown:  return "PowerOff";
        case power_button_id::reboot:    return "Reboot";
        case power_button_id::suspend:   return "Suspend";
        case power_button_id::hibernate: return "Hibernate";
        }

        QCRITICAL() << "DBus method unclear, power_button_id:" << static_cast<int>(id);
        return {};
}

static void handleButtonResolutionFailure(PowerButtonParams&       button,
                                          const PowerButtonParams* defaults, size_t button_index) {
        if (!defaults) {
                QWARNING() << "Failed to default button" << button_index << ", defaults missing!";
                return;
        }

        button = *defaults;
        return;
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
void ConfigMapper::mapCommandArgument(node_view argument_node, PowerButtonParams& button,
                                      const PowerButtonParams* defaults, QStringList& arguments,
                                      size_t button_index, size_t arg_index,
                                      const QString& path_context) {
        QString        argument{};
        constexpr bool is_override = false;

        auto argument_result = resolve<QString>({Source{argument_node, applet::power_applet.scope}},
                                                path_context, is_override);
        if (!argument_result) {
                handleButtonResolutionFailure(button, defaults, button_index);
                return;
        } else {
                argument = argument_result.value();
        }

        if (argument.isEmpty()) { return; }

        arguments << std::move(argument);
}

void ConfigMapper::mapCommandArguments(node_view arguments_node, PowerButtonParams& button,
                                       const PowerButtonParams* defaults, QStringList& arguments,
                                       size_t button_index, const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size    = 0;
        const auto args = resolve<toml::array>({Source{arguments_node, applet::power_applet.scope}},
                                               path_context, is_override,
                                               {"Format: [string, array]", min_size, std::nullopt});
        if (!args) {
                handleButtonResolutionFailure(button, defaults, button_index);
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

void ConfigMapper::mapCommand(node_view command_node, PowerButtonParams& button,
                              const PowerButtonParams* defaults, ShellCommand& command,
                              size_t button_index, const QString& path_context) {
        constexpr bool   is_override = false;
        constexpr size_t min_size = 2, max_size = 2;
        const auto command_arr = resolve<toml::array>({Source{command_node,
                                                              applet::power_applet.scope}},
                                                      path_context, is_override,
                                                      {"Format: [program, [args...]]", min_size,
                                                       max_size});
        if (!command_arr) {
                handleButtonResolutionFailure(button, defaults, button_index);
                return;
        }

        ShellCommand cmd{};

        auto program_result = resolve<QString>({Source{toml::node_view(command_arr.value()[0]),
                                                       applet::power_applet.scope}},
                                               extendCfgPath(path_context, "program"), is_override);
        if (!program_result) {
                handleButtonResolutionFailure(button, defaults, button_index);
                return;
        } else {
                cmd.program = program_result.value();
        }

        mapCommandArguments(toml::node_view(command_arr.value()[1]), button, defaults,
                            cmd.arguments, button_index, extendCfgPath(path_context, "arguments"));

        command = std::move(cmd);
}

bool ConfigMapper::mapPrimaryButton(node_view                             button_params_node,
                                    std::vector<PowerButtonParams>&       buttons,
                                    const std::vector<PowerButtonParams>& default_buttons,
                                    const PowerButtonParams* defaults, size_t button_index,
                                    const QString& path_context) {
        const QString  button_path_context = path_context + QString("[%1]").arg(button_index);
        constexpr bool is_override         = false;

        const auto button_table = resolve<toml::table>({Source{button_params_node,
                                                               applet::power_applet.scope}},
                                                       button_path_context, is_override);
        if (!button_table) {
                buttons = default_buttons;
                return true;
        }

        PowerButtonParams button{};

        auto enabled = resolve<bool>({Source{button_table.value()["enabled"],
                                             applet::power_applet.scope}},
                                     path_context, is_override);
        if (!enabled) { return true; }

        auto id_result = resolve<QString>({Source{button_table.value()["id"],
                                                  applet::power_applet.scope}},
                                          extendCfgPath(path_context, "id"), is_override);
        if (!id_result) {
                handleButtonResolutionFailure(button, defaults, button_index);
                return false;
        } else {
                button.id = getPowerButtonIDFromString(id_result.value());
        }

        auto label_result = resolve<QString>({Source{button_table.value()["label"],
                                                     applet::power_applet.scope}},
                                             extendCfgPath(path_context, "label"), is_override);
        if (!label_result) {
                handleButtonResolutionFailure(button, defaults, button_index);
                return false;
        } else {
                button.label = label_result.value();
        }

        auto order_result = resolve<int64_t>({Source{button_table.value()["order"],
                                                     applet::power_applet.scope}},
                                             extendCfgPath(path_context, "order"), is_override);
        if (!order_result) {
                handleButtonResolutionFailure(button, defaults, button_index);
                return false;
        } else {
                button.order = order_result.value();
        }

        mapCommand(button_params_node["command"], button, defaults, button.command, button_index,
                   extendCfgPath(path_context, "command"));

        button.icon = iconFor(button.id);

        button.dbus_method = dbusMethodFor(button.id);

        buttons.insert(buttons.cend(), std::move(button));

        return false;
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
                defaulted = mapPrimaryButton(node_view(buttons.value().at(i)), buttons_found,
                                             defaults, default_button, i,
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
                  [](const PowerButtonParams& a, const PowerButtonParams& b) -> bool {
                          return a.order < b.order;
                  });

        // Prevent index out of bound issues and multiple buttons with the same order by
        // re-mapping order to a range from 1 to the total number of buttons
        for (size_t i = 0; i != buttons_found.size(); ++i) { buttons_found[i].order = i + 1; }

        primary_buttons = std::move(buttons_found);
}

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

void ConfigMapper::mapToPowerAppletConfig(const toml::table& power_applet_table,
                                          const toml::table& global_table,
                                          PowerAppletConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefault();

        /* Window Properties */
        mapWindow(NodePair{power_applet_table["window"], global_table["window"]},
                  config.window_properties, defaults.getWindowProperties(), "window");

        /* Primary Button Properties */
        mapPrimaryButton(NodePair{power_applet_table["primary_button"],
                                  global_table["primary_button"]},
                         config.primary_button_properties, defaults.getPrimaryButtonProperties(),
                         "primary_button");

        /* Layout Properties */
        mapLayout(power_applet_table["layout"], config.layout_properties,
                  defaults.getLayoutProperties(), "layout");

        /* Environment Properties */
        mapEnvironment(power_applet_table["environment"], config.environment_properties,
                       defaults.getEnvironmentProperties(), "environment");
}
