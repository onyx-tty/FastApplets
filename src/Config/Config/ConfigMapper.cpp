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
#include "Config/Config/PowerApplet/PowerAppletConfig.h"
#include "Config/Config/Properties/EnvironmentProperties.h"
#include "Config/Config/Properties/LayoutProperties.h"
#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Config/Config/Properties/WindowProperties.h"
#include "Config/Keys/PowerApplet/PowerAppletKeys.h"
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

class GlobalConfig;

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
                           const Qt::Alignment& fallback, const QString& path) {
        return getValueFromEnumMap<Qt::Alignment>(key, map, fallback, path);
}

std::optional<Qt::Alignment> tryGetAlignment(const std::string             key,
                                             const EnumMap<Qt::Alignment>& map,
                                             const QString&                path) {
        return tryGetValueFromEnumMap<Qt::Alignment>(key, map, path);
}

QSizePolicy getSizePolicy(const std::string key, const EnumMap<QSizePolicy>& map,
                          const QSizePolicy& fallback, const QString& path) {
        return getValueFromEnumMap<QSizePolicy>(key, map, fallback, path);
}

std::optional<QSizePolicy> tryGetSizePolicy(const std::string key, const EnumMap<QSizePolicy>& map,
                                            const QString& path) {
        return tryGetValueFromEnumMap<QSizePolicy>(key, map, path);
}

static QString makeCfgPath(const char* scope, const QString& config_path,
                           const char* separator = ".") {
        return QString("in config.toml, %1%2%3").arg(scope, separator, config_path);
}

static QString extendCfgPath(const QString& path, const char* extension,
                             const char* separator = ".") {
        if (path.isEmpty()) { return QString(extension); }

        return path + separator + extension;
}

/* Window Properties */
void ConfigMapper::mapWindowSize(node_view size_node, node_view global_fallback_node, QSize& size,
                                 const QString& path_context) {
        // Look for PowerApplet overrides first
        auto data = tryGetQSize(size_node, makeCfgPath("power_applet", path_context));
        if (data) {
                size = data.value();
                return;
        }

        // Fall back to global if not found
        data = tryGetQSize(global_fallback_node, makeCfgPath("global", path_context));
        if (data) {
                size = data.value();
                return;
        }

        // Use hardcoded defaults
        size = PowerAppletConfig::getDefaultPowerAppletConfig().getWindowProperties().getSize();
}

void ConfigMapper::mapWindowTitle(node_view title_node, node_view global_fallback_node,
                                  QString& title, const QString& path_context) {
        // Look for PowerApplet overrides first
        auto data = tryGet<std::string>(title_node, makeCfgPath("power_applet", path_context));
        if (data) {
                title = QString::fromStdString(data.value());
                return;
        }

        // Fall back to global if not found
        data = tryGet<std::string>(global_fallback_node, makeCfgPath("global", path_context));
        if (data) {
                title = QString::fromStdString(data.value());
                return;
        }

        // Use hardcoded defaults
        title = PowerAppletConfig::getDefaultPowerAppletConfig().getWindowProperties().getTitle();
}

void ConfigMapper::mapWindowProperties(node_view window_node, node_view global_fallback_node,
                                       WindowProperties& window, const QString& path_context) {
        // Forward power_data and global_data
        auto* power_data  = getTomlTable(window_node, makeCfgPath("power_applet", path_context));
        auto* global_data = getTomlTable(global_fallback_node, makeCfgPath("global", path_context));
        if (global_data || power_data) {
                node_view power_node  = power_data ? node_view(*power_data) : node_view();
                node_view global_node = global_data ? node_view(*global_data) : node_view();

                mapWindowSize(power_node["size"], global_node["size"], window.size,
                              extendCfgPath(path_context, "size"));
                mapWindowTitle(power_node["title"], global_node["title"], window.title,
                               extendCfgPath(path_context, "title"));

                return;
        }

        // Use hardcoded defaults
        window = PowerAppletConfig::getDefaultPowerAppletConfig().getWindowProperties();
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButtonTextAlignment(node_view      text_alignment_node,
                                                 node_view      global_fallback_node,
                                                 Qt::Alignment& text_alignment,
                                                 const QString& path_context) {
        // Look for PowerApplet overrides first
        QString path = makeCfgPath("power_applet", path_context);
        auto    data = tryGet<std::string>(text_alignment_node, path);
        if (data) {
                auto data_alignment = tryGetAlignment(data.value(), alignment_map, path);

                if (data_alignment) {
                        text_alignment = data_alignment.value();
                        return;
                }
        }

        // Fall back to global if not found
        path = makeCfgPath("global", path_context);
        data = tryGet<std::string>(global_fallback_node, path);
        if (data) {
                auto data_alignment = tryGetAlignment(data.value(), alignment_map, path);

                if (data_alignment) {
                        text_alignment = data_alignment.value();
                        return;
                }
        }

        // Use hardcoded defaults
        text_alignment = PowerAppletConfig::getDefaultPowerAppletConfig()
                                 .getPrimaryButtonProperties()
                                 .getTextAlignment();
}

// TODO This option doesn't work, fix
void ConfigMapper::mapPrimaryButtonIconAlignment(node_view      icon_alignment_node,
                                                 node_view      global_fallback_node,
                                                 Qt::Alignment& icon_alignment,
                                                 const QString& path_context) {
        // Look for PowerApplet overrides first
        QString path(makeCfgPath("power_applet", path_context));
        auto    data = tryGet<std::string>(icon_alignment_node, path);
        if (data) {
                auto data_alignment = tryGetAlignment(data.value(), alignment_map, path);

                if (data_alignment) {
                        icon_alignment = data_alignment.value();
                        return;
                }
        }

        // Fall back to global if not found
        path = makeCfgPath("global", path_context);
        data = tryGet<std::string>(global_fallback_node, path);
        if (data) {
                auto data_alignment = tryGetAlignment(data.value(), alignment_map, path);

                if (data_alignment) {
                        icon_alignment = data_alignment.value();
                        return;
                }
        }

        // Use hardcoded defaults
        icon_alignment = PowerAppletConfig::getDefaultPowerAppletConfig()
                                 .getPrimaryButtonProperties()
                                 .getIconAlignment();
}

void ConfigMapper::mapPrimaryButtonIconSize(node_view icon_size_node,
                                            node_view global_fallback_node, QSize& icon_size,
                                            const QString& path_context) {
        // Look for PowerApplet overrides first
        auto data = tryGetQSize(icon_size_node, makeCfgPath("power_applet", path_context));
        if (data) {
                icon_size = data.value();
                return;
        }

        // Fall back to global if not found
        data = tryGetQSize(global_fallback_node, makeCfgPath("global", path_context));
        if (data) {
                icon_size = data.value();
                return;
        }

        // Use hardcoded defaults
        icon_size = PowerAppletConfig::getDefaultPowerAppletConfig()
                            .getPrimaryButtonProperties()
                            .getIconSize();
}

void ConfigMapper::mapPrimaryButtonPolicy(node_view policy_node, node_view global_fallback_node,
                                          QSizePolicy& policy, const QString& path_context) {
        // Look for PowerApplet overrides first
        QString path = makeCfgPath("power_applet", path_context);
        auto    data = tryGet<std::string>(policy_node, path);
        if (data) {
                auto data_policy = tryGetSizePolicy(data.value(), size_policy_map, path);

                if (data_policy) {
                        policy = data_policy.value();
                        return;
                }
        }

        // Fall back to global if not found
        path = makeCfgPath("global", path_context);
        data = tryGet<std::string>(global_fallback_node, path);
        if (data) {
                auto data_policy = tryGetSizePolicy(data.value(), size_policy_map, path);

                if (data_policy) {
                        policy = data_policy.value();
                        return;
                }
        }

        // Use hardcoded defaults
        policy = PowerAppletConfig::getDefaultPowerAppletConfig()
                         .getPrimaryButtonProperties()
                         .getPolicy();
}

void ConfigMapper::mapPrimaryButtonProperties(node_view button_node, node_view global_fallback_node,
                                              PrimaryButtonProperties& button,
                                              const QString&           path_context) {
        // Forward power_data and global_data
        auto* power_data  = getTomlTable(button_node, makeCfgPath("power_applet", path_context));
        auto* global_data = getTomlTable(global_fallback_node, makeCfgPath("global", path_context));
        if (global_data || power_data) {
                node_view power_node  = power_data ? node_view(*power_data) : node_view();
                node_view global_node = global_data ? node_view(*global_data) : node_view();

                mapPrimaryButtonTextAlignment(power_node["text_alignment"],
                                              global_node["text_alignment"], button.text_alignment,
                                              extendCfgPath(path_context, "text_alignment"));
                mapPrimaryButtonIconAlignment(power_node["icon_alignment"],
                                              global_node["icon_alignment"], button.icon_alignment,
                                              extendCfgPath(path_context, "icon_alignment"));
                mapPrimaryButtonIconSize(power_node["icon_size"], global_node["icon_size"],
                                         button.icon_size,
                                         extendCfgPath(path_context, "icon_size"));
                mapPrimaryButtonPolicy(power_node["policy"], global_node["policy"], button.policy,
                                       extendCfgPath(path_context, "policy"));

                return;
        }

        // Use hardcoded defaults
        button = PowerAppletConfig::getDefaultPowerAppletConfig().getPrimaryButtonProperties();
}

/* Layout Properties */
void ConfigMapper::mapLayoutPrimaryButtonID(node_view id_node, PrimaryButtonData& button,
                                            power_button_id& id, size_t button_index,
                                            const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        auto result = QString::fromStdString(
                getOrDefault<std::string>(id_node, {}, makeCfgPath("power_applet", path_context)));

        if (result.isEmpty()) {
                button = defaults;
                return;
        }

        id = getPowerButtonIDFromString(std::move(result));
}

void ConfigMapper::mapLayoutPrimaryButtonLabel(node_view label_node, PrimaryButtonData& button,
                                               QString& label, size_t button_index,
                                               const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        auto result = QString::fromStdString(
                getOrDefault<std::string>(label_node, {},
                                          makeCfgPath("power_applet", path_context)));

        if (result.isEmpty()) {
                button = defaults;
                return;
        }

        label = result;
}

void ConfigMapper::mapLayoutPrimaryButtonOrder(node_view order_node, PrimaryButtonData& button,
                                               long& order, std::vector<PrimaryButtonData>& buttons,
                                               size_t button_index, const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        // Workaround that creates a unique number corresponding to a default from the largest
        // order in all primary power buttons + 1
        // It will overflow if some of the buttons define the largest order possible for a long
        // TODO Eliminate the button order workaround
        long default_order = 0;
        for (const auto& button_data : buttons) {
                default_order = std::max(default_order, button_data.order + 1);
        }
        const long result = getOrDefault<int64_t>(order_node, default_order,
                                                  makeCfgPath("power_applet", path_context));

        if (result == default_order) {
                button = defaults;
                return;
        }

        order = result;
}

void ConfigMapper::mapLayoutPrimaryButtonCommandProgram(node_view          program_node,
                                                        PrimaryButtonData& button, QString& program,
                                                        size_t         button_index,
                                                        const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        program = QString::fromStdString(
                getOrDefault<std::string>(program_node, defaults.command.program.toStdString(),
                                          makeCfgPath("power_applet", path_context)));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArgumentsArgument(
        node_view argument_node, PrimaryButtonData& button, QStringList& arguments,
        size_t button_index, size_t arg_index, const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        arguments.insert(arguments.cend(),
                         QString::fromStdString(getOrDefault<std::string>(
                                 argument_node, defaults.command.arguments[arg_index].toStdString(),
                                 makeCfgPath("power_applet", path_context))));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArguments(node_view          arguments_node,
                                                          PrimaryButtonData& button,
                                                          QStringList&       arguments,
                                                          size_t             button_index,
                                                          const QString&     path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        constexpr size_t min_size = 0;
        const auto       args     = getTomlArray(arguments_node, min_size,
                                                 makeCfgPath("power_applet", path_context),
                                                 "Format: [string, array]");
        if (!args) {
                button = defaults;
                return;
        }

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapLayoutPrimaryButtonCommandArgumentsArgument(
                        toml::node_view(args.value()[i]), button, arguments, button_index, i,
                        makeCfgPath("power_applet", path_context)
                                + QString("[%1]").arg(i).toStdString().c_str());
        }
}

void ConfigMapper::mapLayoutPrimaryButtonCommand(node_view command_node, PrimaryButtonData& button,
                                                 ShellCommand& command, size_t button_index,
                                                 const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_arr_details = "Format: [program, [args...]]";

        constexpr size_t min_size = 2, max_size = 2;
        const auto       command_arr = getTomlArray(command_node, min_size, max_size,
                                                    makeCfgPath("power_applet", path_context),
                                                    error_arr_details);
        if (!command_arr) {
                button = defaults;
                return;
        }

        mapLayoutPrimaryButtonCommandProgram(toml::node_view(command_arr.value()[0]), button,
                                             command.program, button_index,
                                             extendCfgPath(path_context, "program"));

        mapLayoutPrimaryButtonCommandArguments(toml::node_view(command_arr.value()[1]), button,
                                               command.arguments, button_index,
                                               extendCfgPath(path_context, "arguments"));
}

void ConfigMapper::mapLayoutPrimaryButtonData(node_view                       button_data_node,
                                              PrimaryButtonData&              button_data,
                                              std::vector<PrimaryButtonData>& buttons,
                                              size_t button_index, const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        mapLayoutPrimaryButtonID(button_data_node["id"], button_data, button_data.id, button_index,
                                 extendCfgPath(path_context, "id"));

        mapLayoutPrimaryButtonLabel(button_data_node["label"], button_data, button_data.label,
                                    button_index, extendCfgPath(path_context, "label"));

        mapLayoutPrimaryButtonOrder(button_data_node["order"], button_data, button_data.order,
                                    buttons, button_index, extendCfgPath(path_context, "order"));

        mapLayoutPrimaryButtonCommand(button_data_node["command"], button_data, button_data.command,
                                      button_index, extendCfgPath(path_context, "command"));
}

// TODO Extract logic shared with mapLayoutPrimaryButtonData
void ConfigMapper::logButtonDisabled(node_view id_node, PrimaryButtonData& button_data,
                                     size_t button_index, const QString& path_context) {
        const auto& data     = id_node.as_string();
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        if (!data) { // TODO Duplication, remove
                QWARNING() << makeCfgPath("power_applet", path_context)
                           << " must be a string! Using defaults...";
                button_data = defaults;
                return;
        }

        QDEBUG() << id_node.as_string()->get() << ": DISABLED";
}

void ConfigMapper::mapLayoutPrimaryButtons(node_view                       primary_buttons_node,
                                           std::vector<PrimaryButtonData>& primary_buttons,
                                           const QString&                  path_context) {
        const auto data = getTomlArray(primary_buttons_node, path_context);

        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons();

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

                const QString button_path_context = path_context + QString("[%1]").arg(index);

                const auto* button = getTomlTable(toml::node_view(button_node),
                                                  makeCfgPath("power_applet", button_path_context));
                if (!button) {
                        primary_buttons = defaults;
                        return;
                }

                const bool enabled = getOrDefault((*button)["enabled"], true,
                                                  extendCfgPath(makeCfgPath("power_applet",
                                                                            button_path_context),
                                                                "enabled"));
                if (enabled) {
                        mapLayoutPrimaryButtonData(toml::node_view(button), button_data,
                                                   buttons_found, index, button_path_context);

                        buttons_found.push_back(std::move(button_data));
                } else {
                        logButtonDisabled((*button)["id"], button_data, index,
                                          extendCfgPath(button_path_context, "id"));
                }
        }

        if (buttons_found.empty()) {
                QWARNING() << makeCfgPath("power_applet", path_context)
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

void ConfigMapper::mapLayoutProperties(node_view layout_node, LayoutProperties& layout,
                                       const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig().getLayoutProperties();

        const auto* data = getTomlTable(layout_node, makeCfgPath("power_applet", path_context));
        if (!data) {
                layout = defaults;
                return;
        }

        // Primary power buttons
        mapLayoutPrimaryButtons((*data)["primary_buttons"], layout.primary_power_buttons,
                                extendCfgPath(path_context, "primary_buttons"));
}

void ConfigMapper::mapEnvironmentDBusMode(node_view dbus_mode_node, bool& dbus_mode,
                                          const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getEnvironmentProperties()
                                       .getDBusMode();

        dbus_mode = getOrDefault(dbus_mode_node, defaults,
                                 makeCfgPath("power_applet", path_context));
}

void ConfigMapper::mapEnvironmentProperties(node_view              environment_node,
                                            EnvironmentProperties& environment,
                                            const QString&         path_context) {
        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig()
                                       .getEnvironmentProperties();

        const auto* data = getTomlTable(environment_node, makeCfgPath("power_applet", path_context));
        if (!data) {
                environment = defaults;
                return;
        }

        // D-Bus mode
        mapEnvironmentDBusMode((*data)["dbus_mode"], environment.dbus_mode,
                               extendCfgPath(path_context, "dbus_mode"));
}

void ConfigMapper::mapToGlobalConfig(const toml::table& config_table, GlobalConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig();

        if (!config_table.contains("global")) {
                QWARNING() << "in config.toml, global missing!";
        }

        /* Window Properties */
        mapWindowProperties(config_table["power_applet"]["window"],
                            config_table["global"]["window"], config.window_properties, "window");

        /* Primary Button Properties */
        mapPrimaryButtonProperties(config_table["power_applet"]["primary_button"],
                                   config_table["global"]["primary_button"],
                                   config.primary_button_properties, "primary_buttons");
}

void ConfigMapper::mapToPowerAppletConfig(const toml::table& config_table,
                                          PowerAppletConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefaultPowerAppletConfig();

        if (!config_table.contains("power_applet")) {
                QWARNING() << "in config.toml, power_applet missing!";
        }

        mapToGlobalConfig(config_table, config);

        /* Layout Properties */
        mapLayoutProperties(config_table["power_applet"]["layout"], config.layout_properties,
                            "layout");

        /* Environment Properties */
        mapEnvironmentProperties(config_table["power_applet"]["environment"],
                                 config.environment_properties, "environment");
}
