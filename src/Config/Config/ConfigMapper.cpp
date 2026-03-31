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

static Qt::Alignment getAlignment(const std::string key, const EnumMap<Qt::Alignment>& map,
                                  const Qt::Alignment& fallback, const QString& path) {
        return getValueFromEnumMap<Qt::Alignment>(key, map, fallback, path);
}

static std::optional<Qt::Alignment> tryGetAlignment(const std::string             key,
                                                    const EnumMap<Qt::Alignment>& map,
                                                    const QString&                path) {
        return tryGetValueFromEnumMap<Qt::Alignment>(key, map, path);
}

static QSizePolicy getSizePolicy(const std::string key, const EnumMap<QSizePolicy>& map,
                                 const QSizePolicy& fallback, const QString& path) {
        return getValueFromEnumMap<QSizePolicy>(key, map, fallback, path);
}

static std::optional<QSizePolicy> tryGetSizePolicy(const std::string           key,
                                                   const EnumMap<QSizePolicy>& map,
                                                   const QString&              path) {
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

namespace extractor {
static auto table = [](node_view node, const QString& path) -> std::optional<toml::table> {
        if (auto* result = getTomlTable(node, path)) { return *result; }

        return std::nullopt;
};

static auto array = [](node_view node, const QString& path, const QString& error_arr_details = {}) {
        return getTomlArray(node, path, error_arr_details);
};

template<typename T>
static auto value = [](node_view node, const QString& path) { return tryGet<T>(node, path); };

static auto qstring = [](node_view node, const QString& path) -> std::optional<QString> {
        if (auto str = extractor::value<std::string>(node, path)) {
                return QString::fromStdString(str.value());
        }

        return std::nullopt;
};

static auto qsize = [](node_view node, const QString& path) { return tryGetQSize(node, path); };

static auto alignment = [](node_view node, const QString& path) -> std::optional<Qt::Alignment> {
        auto raw = tryGet<std::string>(node, path);
        if (!raw) { return std::nullopt; }
        return tryGetAlignment(raw.value(), alignment_map, path);
};

static auto size_policy = [](node_view node, const QString& path) -> std::optional<QSizePolicy> {
        auto raw = tryGet<std::string>(node, path);
        if (!raw) { return std::nullopt; }
        return tryGetSizePolicy(raw.value(), size_policy_map, path);
};
} // namespace extractor

struct Source final {
        node_view   node;
        const char* scope;
};

template<typename T>
static std::optional<T> resolve(std::initializer_list<Source> sources, const QString& path_context) {
        using DT = std::decay_t<T>;

        // Collapse extraction logic into that of a corresponding type
        static auto extract = [&](node_view node, const QString& path) -> std::optional<DT> {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        return extractor::table(node, path);
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        return extractor::array(node, path);
                } else if constexpr (std::is_same_v<DT, QSize>) {
                        return extractor::qsize(node, path);
                } else if constexpr (std::is_same_v<DT, Qt::Alignment>) {
                        return extractor::alignment(node, path);
                } else if constexpr (std::is_same_v<DT, QSizePolicy>) {
                        return extractor::size_policy(node, path);
                } else if constexpr (std::is_same_v<DT, QString>) {
                        return extractor::qstring(node, path);
                } else {
                        return extractor::value<DT>(node, path);
                }
        };

        // Validate and attempt extraction of each passed source, prioritizing earliest ones
        for (const auto& source : sources) {
                if (auto result = extract(source.node, makeCfgPath(source.scope, path_context))) {
                        return *result;
                }
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}

template<typename T, typename... Sources>
static std::optional<T> resolve(const QString& path_context, Sources&&... sources) {
        return resolve<T>({std::forward<Sources>(sources)...}, path_context);
}

template<typename T, typename DefaultT>
static T resolveOr(std::initializer_list<Source> sources, const DefaultT& defaults,
                   const QString& path_context) {
        return resolve<T>(sources, path_context).value_or(defaults);
}

template<typename T, typename DefaultT, typename... Sources>
static T resolveOr(const QString& path_context, const DefaultT& defaults, Sources&&... sources) {
        return resolveOr<T>({std::forward<Sources>(sources)...}, defaults, path_context);
}

/* Window Properties */
void ConfigMapper::mapWindowSize(node_view size_node, node_view global_fallback_node, QSize& size,
                                 const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault().getWindowProperties().getSize();

        size = resolveOr<QSize>(path_context, defaults, Source{size_node, "power_applet"},
                                Source{global_fallback_node, "global"});
}

void ConfigMapper::mapWindowTitle(node_view title_node, node_view global_fallback_node,
                                  QString& title, const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault().getWindowProperties().getTitle();

        title = QString::fromStdString(
                resolveOr<std::string>(path_context, defaults.toStdString(),
                                       Source{title_node, "power_applet"},
                                       Source{global_fallback_node, "global"}));
}

void ConfigMapper::mapWindowProperties(node_view window_node, node_view global_fallback_node,
                                       WindowProperties& window, const QString& path_context) {
        // Resolve power_data and global_data
        auto power_data  = resolve<toml::table>(path_context, Source{window_node, "power_applet"});
        auto global_data = resolve<toml::table>(path_context,
                                                Source{global_fallback_node, "global"});

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) {
                window = PowerAppletConfig::getDefault().getWindowProperties();
                return;
        }

        node_view power_node  = power_data ? node_view(power_data.value()) : node_view();
        node_view global_node = global_data ? node_view(global_data.value()) : node_view();

        WindowProperties window_properties{};

        mapWindowSize(power_node["size"], global_node["size"], window_properties.size,
                      extendCfgPath(path_context, "size"));
        mapWindowTitle(power_node["title"], global_node["title"], window_properties.title,
                       extendCfgPath(path_context, "title"));

        window = std::move(window_properties);
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButtonTextAlignment(node_view      text_alignment_node,
                                                 node_view      global_fallback_node,
                                                 Qt::Alignment& text_alignment,
                                                 const QString& path_context) {
        const auto& defaults =
                PowerAppletConfig::getDefault().getPrimaryButtonProperties().getTextAlignment();

        text_alignment = resolveOr<Qt::Alignment>(path_context, defaults,
                                                  Source{text_alignment_node, "power_applet"},
                                                  Source{global_fallback_node, "global"});
}

// TODO This option doesn't work, fix
void ConfigMapper::mapPrimaryButtonIconAlignment(node_view      icon_alignment_node,
                                                 node_view      global_fallback_node,
                                                 Qt::Alignment& icon_alignment,
                                                 const QString& path_context) {
        const auto& defaults =
                PowerAppletConfig::getDefault().getPrimaryButtonProperties().getIconAlignment();

        icon_alignment = resolveOr<Qt::Alignment>(path_context, defaults,
                                                  Source{icon_alignment_node, "power_applet"},
                                                  Source{global_fallback_node, "global"});
}

void ConfigMapper::mapPrimaryButtonIconSize(node_view icon_size_node,
                                            node_view global_fallback_node, QSize& icon_size,
                                            const QString& path_context) {
        const auto& defaults =
                PowerAppletConfig::getDefault().getPrimaryButtonProperties().getIconSize();

        icon_size = resolveOr<QSize>(path_context, defaults, Source{icon_size_node, "power_applet"},
                                     Source{global_fallback_node, "global"});
}

void ConfigMapper::mapPrimaryButtonPolicy(node_view policy_node, node_view global_fallback_node,
                                          QSizePolicy& policy, const QString& path_context) {
        const auto& defaults =
                PowerAppletConfig::getDefault().getPrimaryButtonProperties().getPolicy();

        policy = resolveOr<QSizePolicy>(path_context, defaults, Source{policy_node, "power_applet"},
                                        Source{global_fallback_node, "global"});
}

void ConfigMapper::mapPrimaryButtonProperties(node_view button_node, node_view global_fallback_node,
                                              PrimaryButtonProperties& button,
                                              const QString&           path_context) {
        // Resolve power_data and global_data
        auto power_data  = resolve<toml::table>(path_context, Source{button_node, "power_applet"});
        auto global_data = resolve<toml::table>(path_context,
                                                Source{global_fallback_node, "global"});

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) {
                button = PowerAppletConfig::getDefault().getPrimaryButtonProperties();
                return;
        }

        node_view power_node  = power_data ? node_view(power_data.value()) : node_view();
        node_view global_node = global_data ? node_view(global_data.value()) : node_view();

        PrimaryButtonProperties button_properties{};

        mapPrimaryButtonTextAlignment(power_node["text_alignment"], global_node["text_alignment"],
                                      button_properties.text_alignment,
                                      extendCfgPath(path_context, "text_alignment"));
        mapPrimaryButtonIconAlignment(power_node["icon_alignment"], global_node["icon_alignment"],
                                      button_properties.icon_alignment,
                                      extendCfgPath(path_context, "icon_alignment"));
        mapPrimaryButtonIconSize(power_node["icon_size"], global_node["icon_size"],
                                 button_properties.icon_size,
                                 extendCfgPath(path_context, "icon_size"));
        mapPrimaryButtonPolicy(power_node["policy"], global_node["policy"],
                               button_properties.policy, extendCfgPath(path_context, "policy"));

        button = std::move(button_properties);
}

/* Layout Properties */
void ConfigMapper::mapLayoutPrimaryButtonID(node_view id_node, PrimaryButtonData& button,
                                            power_button_id& id, size_t button_index,
                                            const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        auto result = resolve<std::string>(path_context, Source{id_node, "power_applet"});

        if (!result) {
                button = defaults;
                return;
        }

        id = getPowerButtonIDFromString(QString::fromStdString(result.value()));
}

void ConfigMapper::mapLayoutPrimaryButtonLabel(node_view label_node, PrimaryButtonData& button,
                                               QString& label, size_t button_index,
                                               const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        auto result = resolve<std::string>(path_context, Source{label_node, "power_applet"});

        if (!result) {
                button = defaults;
                return;
        }

        label = QString::fromStdString(result.value());
}

void ConfigMapper::mapLayoutPrimaryButtonOrder(node_view order_node, PrimaryButtonData& button,
                                               long& order, std::vector<PrimaryButtonData>& buttons,
                                               size_t button_index, const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        const auto result = resolve<int64_t>(path_context, Source{order_node, "power_applet"});

        if (!result) {
                button = defaults;
                return;
        }

        order = result.value();
}

void ConfigMapper::mapLayoutPrimaryButtonCommandProgram(node_view          program_node,
                                                        PrimaryButtonData& button, QString& program,
                                                        size_t         button_index,
                                                        const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        const auto result = resolve<std::string>(path_context, Source{program_node, "power_applet"});
        if (!result) {
                button = defaults;
                return;
        }

        program = QString::fromStdString(result.value());
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArgumentsArgument(
        node_view argument_node, PrimaryButtonData& button, QStringList& arguments,
        size_t button_index, size_t arg_index, const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        const auto result = resolve<std::string>(path_context,
                                                 Source{argument_node, "power_applet"});
        if (!result) {
                button = defaults;
                return;
        }

        arguments.insert(arguments.cend(), QString::fromStdString(result.value()));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArguments(node_view          arguments_node,
                                                          PrimaryButtonData& button,
                                                          QStringList&       arguments,
                                                          size_t             button_index,
                                                          const QString&     path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        constexpr size_t min_size = 0;
        const auto args = getTomlArray(arguments_node, makeCfgPath("power_applet", path_context),
                                       "Format: [string, array]", min_size);
        if (!args) {
                button = defaults;
                return;
        }

        QStringList argument_list{};

        for (size_t i = 0; i != args.value().size(); ++i) {
                mapLayoutPrimaryButtonCommandArgumentsArgument(
                        toml::node_view(args.value()[i]), button, argument_list, button_index, i,
                        makeCfgPath("power_applet", path_context)
                                + QString("[%1]").arg(i).toStdString().c_str());
        }

        arguments = std::move(argument_list);
}

void ConfigMapper::mapLayoutPrimaryButtonCommand(node_view command_node, PrimaryButtonData& button,
                                                 ShellCommand& command, size_t button_index,
                                                 const QString& path_context) {
        const auto& defaults = PowerAppletConfig::getDefault()
                                       .getLayoutProperties()
                                       .getPrimaryPowerButtons()[button_index];

        QString error_arr_details = "Format: [program, [args...]]";

        constexpr size_t min_size = 2, max_size = 2;
        const auto       command_arr = getTomlArray(command_node,
                                                    makeCfgPath("power_applet", path_context),
                                                    error_arr_details, min_size, max_size);
        if (!command_arr) {
                button = defaults;
                return;
        }

        ShellCommand cmd{};

        mapLayoutPrimaryButtonCommandProgram(toml::node_view(command_arr.value()[0]), button,
                                             cmd.program, button_index,
                                             extendCfgPath(path_context, "program"));

        mapLayoutPrimaryButtonCommandArguments(toml::node_view(command_arr.value()[1]), button,
                                               cmd.arguments, button_index,
                                               extendCfgPath(path_context, "arguments"));

        command = std::move(cmd);
}

bool ConfigMapper::mapLayoutPrimaryButtonData(node_view                       button_data_node,
                                              std::vector<PrimaryButtonData>& buttons,
                                              size_t button_index, const QString& path_context) {
        if (button_index > buttons.size()) {
                if (button_index > 1) {
                        QFATAL("The button after %s is too high! Index is %zu",
                               buttons[button_index].label, button_index);
                }

                QFATAL("The button index is too high! %zu", button_index);
        }

        const auto& default_buttons =
                PowerAppletConfig::getDefault().getLayoutProperties().getPrimaryPowerButtons();
        const auto&   defaults            = default_buttons[button_index];
        const QString button_path_context = path_context + QString("[%1]").arg(button_index);

        const auto button_table = resolve<toml::table>(button_path_context,
                                                       Source{button_data_node, "power_applet"});
        if (!button_table) {
                buttons = default_buttons;
                return true;
        }

        const auto enabled = resolve<bool>(extendCfgPath(button_path_context, "enabled"),
                                           Source{button_table.value()["enabled"], "power_applet"});

        if (!enabled || !enabled.value()) {
                QString info = "Button %1: DISABLED";
                if (const auto label = (button_table.value())["label"].as_string()) {
                        QDEBUG() << info.arg(label->get());
                } else {
                        QDEBUG() << info.arg(button_index);
                }

                return true;
        }

        PrimaryButtonData button{};

        mapLayoutPrimaryButtonID(button_data_node["id"], button, button.id, button_index,
                                 extendCfgPath(path_context, "id"));

        mapLayoutPrimaryButtonLabel(button_data_node["label"], button, button.label, button_index,
                                    extendCfgPath(path_context, "label"));

        mapLayoutPrimaryButtonOrder(button_data_node["order"], button, button.order, buttons,
                                    button_index, extendCfgPath(path_context, "order"));

        mapLayoutPrimaryButtonCommand(button_data_node["command"], button, button.command,
                                      button_index, extendCfgPath(path_context, "command"));

        buttons.insert(buttons.cend(), std::move(button));

        return false;
}

void ConfigMapper::mapLayoutPrimaryButtons(node_view                       primary_buttons_node,
                                           std::vector<PrimaryButtonData>& primary_buttons,
                                           const QString&                  path_context) {
        const auto& defaults =
                PowerAppletConfig::getDefault().getLayoutProperties().getPrimaryPowerButtons();
        const auto buttons = resolve<toml::array>(path_context,
                                                  Source{primary_buttons_node, "power_applet"});
        if (!buttons) {
                primary_buttons = defaults;
                return;
        }

        std::vector<PrimaryButtonData> buttons_found{};

        bool defaulted = false;
        for (size_t i = 0; i != buttons.value().size(); ++i) {
                defaulted = mapLayoutPrimaryButtonData(node_view(buttons.value().at(i)),
                                                       buttons_found, i,
                                                       extendCfgPath(path_context,
                                                                     std::to_string(i).c_str()));
                if (defaulted) { return; }
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
        const auto&      defaults = PowerAppletConfig::getDefault().getLayoutProperties();
        LayoutProperties layout_properties{};

        const auto data = resolve<toml::table>(path_context, Source{layout_node, "power_applet"});
        if (!data) {
                layout_properties = defaults;
                layout            = std::move(layout_properties);
                return;
        }

        // Primary power buttons
        mapLayoutPrimaryButtons(data.value()["primary_buttons"],
                                layout_properties.primary_power_buttons,
                                extendCfgPath(path_context, "primary_buttons"));

        layout = std::move(layout_properties);
}

void ConfigMapper::mapEnvironmentDBusMode(node_view dbus_mode_node, bool& dbus_mode,
                                          const QString& path_context) {
        const auto& defaults =
                PowerAppletConfig::getDefault().getEnvironmentProperties().getDBusMode();

        const auto result = resolve<bool>(path_context, Source{dbus_mode_node, "power_applet"});
        if (!result) {
                dbus_mode = defaults;
                return;
        }

        dbus_mode = result.value();
}

void ConfigMapper::mapEnvironmentProperties(node_view              environment_node,
                                            EnvironmentProperties& environment,
                                            const QString&         path_context) {
        const auto&           defaults = PowerAppletConfig::getDefault().getEnvironmentProperties();
        EnvironmentProperties environment_properties{};

        const auto data = resolve<toml::table>(path_context,
                                               Source{environment_node, "power_applet"});
        if (!data) {
                environment_properties = defaults;
                environment            = std::move(environment_properties);
                return;
        }

        // D-Bus mode
        mapEnvironmentDBusMode(data.value()["dbus_mode"], environment_properties.dbus_mode,
                               extendCfgPath(path_context, "dbus_mode"));

        environment = std::move(environment_properties);
}

void ConfigMapper::mapToGlobalConfig(const toml::table& config_table, GlobalConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefault();

        if (!config_table.contains("global")) { QWARNING() << "in config.toml, global missing!"; }

        /* Window Properties */
        mapWindowProperties(config_table["power_applet"]["window"],
                            config_table["global"]["window"], config.window_properties, "window");

        /* Primary Button Properties */
        mapPrimaryButtonProperties(config_table["power_applet"]["primary_button"],
                                   config_table["global"]["primary_button"],
                                   config.primary_button_properties, "primary_button");
}

void ConfigMapper::mapToPowerAppletConfig(const toml::table& config_table,
                                          PowerAppletConfig& config) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instanceExists()) {
                QFATAL("QApplication has not been instantiated yet!");
        }

        const auto& defaults = PowerAppletConfig::getDefault();

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
