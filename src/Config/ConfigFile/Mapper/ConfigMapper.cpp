// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/ConfigFile/Properties/EnvironmentProperties.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Types/NodePair.h"
#include "Config/Types/NodeView.h"
#include "CppUtils/Log/QtLog.h"
#include "TomlQt/ArrayBounds.h"
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
static T mapProperties(NodePair nodes, const T& defaults, const PathContext& path_context,
                       auto fill_fn) {
        constexpr bool quiet = true;

        // Resolve power_data and global_data
        auto power_data = Resolver::from<toml::table>({Source{nodes.primary,
                                                              applet::power_applet.scope, quiet}},
                                                      path_context);

        auto global_data = Resolver::from<toml::table>({Source{nodes.fallback,
                                                               applet::global.scope}},
                                                       path_context);

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) { return defaults; }

        T props{};
        fill_fn(nodes, props, path_context);
        return std::move(props);
}

/* Window Properties */
void ConfigMapper::mapWindow(NodePair nodes, WindowProperties& window,
                             const WindowProperties& defaults, const PathContext& path_context) {
        window = mapProperties(
                nodes, defaults, path_context,
                [&defaults](NodePair nodes, WindowProperties& window,
                            const PathContext& path_context) {
                        window.size = Resolver::fromOr<QSize>({Source{nodes.primary["size"],
                                                                      applet::power_applet.scope},
                                                               Source{nodes.fallback["size"],
                                                                      applet::global.scope}},
                                                              defaults.getSize(),
                                                              path_context.getExtended("size"));

                        window.title = Resolver::fromOr<QString>({Source{nodes.primary["title"],
                                                                         applet::power_applet.scope},
                                                                  Source{nodes.fallback["title"],
                                                                         applet::global.scope}},
                                                                 defaults.getTitle(),
                                                                 path_context.getExtended("title"));
                });
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                    const PrimaryButtonProperties& defaults,
                                    const PathContext&             path_context) {
        button = mapProperties(
                nodes, defaults, path_context,
                [&defaults](NodePair nodes, PrimaryButtonProperties& button,
                            const PathContext& path_context) {
                        button.text_alignment = Resolver::fromOr<Qt::Alignment>(
                                {Source{nodes.primary["text_alignment"], applet::power_applet.scope},
                                 Source{nodes.fallback["text_alignment"], applet::global.scope}},
                                defaults.getTextAlignment(),
                                path_context.getExtended("text_alignment"));

                        button.icon_alignment = Resolver::fromOr<Qt::Alignment>(
                                {Source{nodes.primary["icon_alignment"], applet::power_applet.scope},
                                 Source{nodes.fallback["icon_alignment"], applet::global.scope}},
                                defaults.getIconAlignment(),
                                path_context.getExtended("icon_alignment"));

                        button.icon_size = Resolver::fromOr<QSize>(
                                {Source{nodes.primary["icon_size"], applet::power_applet.scope},
                                 Source{nodes.fallback["icon_size"], applet::global.scope}},
                                defaults.getIconSize(), path_context.getExtended("icon_size"));

                        button.policy = Resolver::fromOr<QSizePolicy>(
                                {Source{nodes.primary["policy"], applet::power_applet.scope},
                                 Source{nodes.fallback["policy"], applet::global.scope}},
                                defaults.getPolicy(), path_context.getExtended("policy"));
                });
}

/* Layout Properties */
void ConfigMapper::mapLayout(node_view layout_node, LayoutProperties& layout,
                             const LayoutProperties& defaults, const PathContext& path_context) {
        LayoutProperties layout_properties{};

        const auto data = Resolver::from<toml::table>({Source{layout_node,
                                                              applet::power_applet.scope}},
                                                      path_context);
        if (!data) {
                layout_properties = defaults;
                layout            = std::move(layout_properties);
                return;
        }

        // Primary power buttons
        mapPrimaryButtons(data.value()["primary_buttons"], layout_properties.power_buttons,
                          defaults.getPowerButtons(), path_context.getExtended("primary_buttons"));

        layout = std::move(layout_properties);
}

void ConfigMapper::mapPrimaryButtons(node_view                             primary_buttons_node,
                                     std::vector<PowerButtonParams>&       primary_buttons,
                                     const std::vector<PowerButtonParams>& defaults,
                                     const PathContext&                    path_context) {
        constexpr size_t min_size = 1;

        const auto buttons = Resolver::from<toml::array>({Source{primary_buttons_node,
                                                                 applet::power_applet.scope}},
                                                         path_context, {min_size},
                                                         "Format: [primary buttons...]");
        if (!buttons) {
                primary_buttons = defaults;
                return;
        }

        std::vector<PowerButtonParams> buttons_found{};

        bool defaulted = false;
        for (size_t i = 0; i != buttons.value().size(); ++i) {
                // If index out of bounds for defaults, pass nullptr
                const auto* default_button = (i < defaults.size()) ? &defaults[i] : nullptr;
                defaulted = mapPrimaryButton(node_view(buttons.value().at(i)), buttons_found,
                                             defaults, path_context.getExtended(i));

                if (defaulted) {
                        primary_buttons = std::move(buttons_found);
                        return;
                }
        }

        if (buttons_found.empty()) {
                QWARNING() << path_context.makePath(applet::power_applet.scope)
                                      + ", no enabled buttons found! Using defaults...";
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
                                    const PathContext&                    path_context) {
        const auto button_table = Resolver::from<toml::table>({Source{button_params_node,
                                                                      applet::power_applet.scope}},
                                                              path_context);
        if (!button_table) {
                buttons = defaults;
                return true;
        }

        PowerButtonParams button{};

        auto enabled = Resolver::from<bool>({Source{button_table.value()["enabled"],
                                                    applet::power_applet.scope}},
                                            path_context.getExtended("enabled"));
        if (!enabled || !enabled.value()) { return false; }

        auto id_result = Resolver::from<QString>({Source{button_table.value()["id"],
                                                         applet::power_applet.scope}},
                                                 path_context.getExtended("id"));
        if (!id_result) {
                buttons = defaults;
                return true;
        }
        button.id = getPowerButtonIDFromString(id_result.value());

        auto text_result = Resolver::from<QString>({Source{button_table.value()["text"],
                                                           applet::power_applet.scope}},
                                                   path_context.getExtended("text"));
        if (!text_result) {
                buttons = defaults;
                return true;
        }
        button.text = text_result.value();

        auto order_result = Resolver::from<int64_t>({Source{button_table.value()["order"],
                                                            applet::power_applet.scope}},
                                                    path_context.getExtended("order"));
        if (!order_result) {
                buttons = defaults;
                return true;
        }
        button.order = order_result.value();

        mapCommand(button_params_node["command"], buttons, defaults, button.command,
                   path_context.getExtended("command"));

        button.icon = iconFor(button.id);

        button.dbus_method = dbusMethodFor(button.id);

        buttons.insert(buttons.cend(), std::move(button));

        return false;
}

void ConfigMapper::mapCommand(node_view command_node, std::vector<PowerButtonParams>& buttons,
                              const std::vector<PowerButtonParams>& defaults, ShellCommand& command,
                              const PathContext& path_context) {
        constexpr size_t min_size = 2, max_size = 2;
        const auto command_arr = Resolver::from<toml::array>({Source{command_node,
                                                                     applet::power_applet.scope}},
                                                             path_context, {min_size, max_size},
                                                             "Format: [program, [args...]]");
        if (!command_arr) {
                buttons = defaults;
                return;
        }

        ShellCommand cmd{};

        auto program_result = Resolver::from<QString>({Source{toml::node_view(
                                                                      command_arr.value()[0]),
                                                              applet::power_applet.scope}},
                                                      path_context.getExtended("program"));
        if (!program_result) {
                buttons = defaults;
                return;
        }
        cmd.program = program_result.value();

        mapCommandArguments(toml::node_view(command_arr.value()[1]), buttons, defaults,
                            cmd.arguments, path_context.getExtended("arguments"));

        command = std::move(cmd);
}

void ConfigMapper::mapCommandArguments(node_view                             arguments_node,
                                       std::vector<PowerButtonParams>&       buttons,
                                       const std::vector<PowerButtonParams>& defaults,
                                       QStringList& arguments, const PathContext& path_context) {
        constexpr size_t min_size = 0;
        const auto       args = Resolver::from<toml::array>({Source{arguments_node,
                                                                    applet::power_applet.scope}},
                                                            path_context, {min_size},
                                                            "Format: [string, array]");
        if (!args) {
                buttons = defaults;
                return;
        }

        QStringList argument_list{};

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapCommandArgument(toml::node_view(args.value()[i]), buttons, defaults,
                                   argument_list, path_context.getExtended(i));
        }

        arguments = std::move(argument_list);
}

void ConfigMapper::mapCommandArgument(node_view                             argument_node,
                                      std::vector<PowerButtonParams>&       buttons,
                                      const std::vector<PowerButtonParams>& defaults,
                                      QStringList& arguments, const PathContext& path_context) {
        QString argument{};

        auto argument_result = Resolver::from<QString>({Source{argument_node,
                                                               applet::power_applet.scope}},
                                                       path_context);
        if (!argument_result) {
                buttons = defaults;
                return;
        }
        argument = argument_result.value();

        if (argument.isEmpty()) { return; }

        arguments << std::move(argument);
}

void ConfigMapper::mapEnvironment(node_view environment_node, EnvironmentProperties& environment,
                                  const EnvironmentProperties& defaults,
                                  const PathContext&           path_context) {
        EnvironmentProperties environment_properties{};

        const auto data = Resolver::from<toml::table>({Source{environment_node,
                                                              applet::power_applet.scope}},
                                                      path_context);
        if (!data) {
                environment_properties = defaults;
                environment            = std::move(environment_properties);
                return;
        }

        // D-Bus mode
        environment_properties.dbus_mode =
                Resolver::fromOr<bool, bool>({Source{data.value()["dbus_mode"],
                                                     applet::power_applet.scope}},
                                             defaults.getDBusMode(),
                                             path_context.getExtended("dbus_mode"));

        environment = std::move(environment_properties);
}
