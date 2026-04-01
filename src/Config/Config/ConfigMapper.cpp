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

// Use if return value and defaulting must be handled manually
// On success: extract from a node, return as std::optional<T>
// On failure: return std::nullopt
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

// Use to skip validation of return value and to automatically default
// On success: extract from a node
// On failure: copy default value
template<typename T, typename DefaultT>
static T resolveOr(std::initializer_list<Source> sources, const DefaultT& defaults,
                   const QString& path_context) {
        return resolve<T>(sources, path_context).value_or(defaults);
}

template<typename T, typename DefaultT, typename... Sources>
static T resolveOr(const QString& path_context, const DefaultT& defaults, Sources&&... sources) {
        return resolveOr<T>({std::forward<Sources>(sources)...}, defaults, path_context);
}

// Use to try and extract a value from a node into a specific attribute, and if that fails, to
// default a completely different object
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: write result into a provided attribute
// On failure: overwrite object with object_defaults entirely
template<typename TAttribute, typename TObject>
void resolveOrDefault(std::initializer_list<Source> sources, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, const QString& path_context) {
        if (auto result = resolve<TAttribute>(sources, path_context)) {
                attribute = result.value();
        } else {
                object = object_defaults;
        }
}

template<typename TAttribute, typename TObject, typename... Sources>
void resolveOrDefault(const QString& path_context, TAttribute& attribute, TObject& object,
                      const TObject& object_defaults, Sources&&... sources) {
        resolveOrDefault<TAttribute, TObject>({std::forward<Sources>(sources)...}, attribute,
                                              object, object_defaults, path_context);
}

// Use if resolveOrDefault is the optimal choice, but the extracted value must first be transformed
// before being put into use
// For example: if button ID is erroneous, default the button itself, not just the id
//
// On success: transform, then write result into an attribute
// On failure: overwrite the object with object_defaults entirely
template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
void resolveTransformOrDefault(std::initializer_list<Source> sources, TAttribute& attribute,
                               TObject& object, const TObject& object_defaults,
                               Transform&& transform, const QString& path_context) {
        if (auto result = resolve<TRaw>(sources, path_context)) {
                attribute = transform(std::move(result.value()));
        } else {
                object = object_defaults;
        }
}

template<typename TRaw, typename TAttribute, typename TObject, typename Transform, typename... Sources>
void resolveTransformOrDefault(const QString& path_context, TAttribute& attribute, TObject& object,
                               const TObject& object_defaults, Transform&& transform,
                               Sources&&... sources) {
        resolveTransformOrDefault<TRaw>({std::forward<Sources>(sources)...}, attribute, object,
                                        object_defaults, transform, path_context);
}

/* Window Properties */
void ConfigMapper::mapWindowProperties(NodePair nodes, WindowProperties& window,
                                       const WindowProperties& defaults,
                                       const QString&          path_context) {
        // Resolve power_data and global_data
        auto power_data = resolve<toml::table>(path_context, Source{nodes.primary, "power_applet"});
        auto global_data = resolve<toml::table>(path_context, Source{nodes.fallback, "global"});

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) {
                window = defaults;
                return;
        }

        node_view power_node  = power_data ? node_view(power_data.value()) : node_view();
        node_view global_node = global_data ? node_view(global_data.value()) : node_view();

        WindowProperties window_properties{};

        window_properties.size = resolveOr<QSize>(extendCfgPath(path_context, "size"),
                                                  defaults.getSize(),
                                                  Source{nodes.primary["size"], "power_applet"},
                                                  Source{nodes.fallback["size"], "global"});

        window_properties.title = resolveOr<QString>(extendCfgPath(path_context, "title"),
                                                     defaults.getTitle(),
                                                     Source{nodes.primary["title"], "power_applet"},
                                                     Source{nodes.fallback["title"],
                                                            "power_applet"});

        window = std::move(window_properties);
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButtonProperties(NodePair nodes, PrimaryButtonProperties& button,
                                              const PrimaryButtonProperties& defaults,
                                              const QString&                 path_context) {
        // Resolve power_data and global_data
        auto power_data = resolve<toml::table>(path_context, Source{nodes.primary, "power_applet"});
        auto global_data = resolve<toml::table>(path_context, Source{nodes.fallback, "global"});

        // Use hardcoded defaults if no tables found
        if (!power_data && !global_data) {
                button = defaults;
                return;
        }

        node_view power_node  = power_data ? node_view(power_data.value()) : node_view();
        node_view global_node = global_data ? node_view(global_data.value()) : node_view();

        PrimaryButtonProperties button_properties{};

        button_properties.text_alignment =
                resolveOr<Qt::Alignment>(extendCfgPath(path_context, "text_alignment"),
                                         defaults.getTextAlignment(),
                                         Source{nodes.primary["text_alignment"], "power_applet"},
                                         Source{nodes.fallback["text_alignment"], "global"});

        // TODO This option doesn't work because icon alignment is not applied anywhere yet, fix
        button_properties.icon_alignment =
                resolveOr<Qt::Alignment>(extendCfgPath(path_context, "icon_alignment"),
                                         defaults.getIconAlignment(),
                                         Source{nodes.primary["icon_alignment"], "power_applet"},
                                         Source{nodes.fallback["icon_alignment"], "global"});

        button_properties.icon_size =
                resolveOr<QSize>(extendCfgPath(path_context, "icon_size"), defaults.getIconSize(),
                                 Source{nodes.primary["icon_size"], "power_applet"},
                                 Source{nodes.fallback["icon_size"], "global"});

        button_properties.policy =
                resolveOr<QSizePolicy>(extendCfgPath(path_context, "policy"), defaults.getPolicy(),
                                       Source{nodes.primary["policy"], "power_applet"},
                                       Source{nodes.fallback["policy"], "global"});

        button = std::move(button_properties);
}

/* Layout Properties */
void ConfigMapper::mapLayoutPrimaryButtonCommandArgumentsArgument(
        node_view argument_node, PrimaryButtonData& button, const PrimaryButtonData& defaults,
        QStringList& arguments, size_t button_index, size_t arg_index,
        const QString& path_context) {
        QString argument{};

        resolveOrDefault<QString>(path_context, argument, button, defaults,
                                  Source{argument_node, "power_applet"});
        if (argument.isEmpty()) { return; }

        arguments.append(std::move(argument));
}

void ConfigMapper::mapLayoutPrimaryButtonCommandArguments(
        node_view arguments_node, PrimaryButtonData& button, const PrimaryButtonData& defaults,
        QStringList& arguments, size_t button_index, const QString& path_context) {
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
                        toml::node_view(args.value()[i]), button, defaults, argument_list,
                        button_index, i,
                        makeCfgPath("power_applet", path_context)
                                + QString("[%1]").arg(i).toStdString().c_str());
        }

        arguments = std::move(argument_list);
}

void ConfigMapper::mapLayoutPrimaryButtonCommand(node_view command_node, PrimaryButtonData& button,
                                                 const PrimaryButtonData& defaults,
                                                 ShellCommand& command, size_t button_index,
                                                 const QString& path_context) {
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

        resolveOrDefault<QString>(extendCfgPath(path_context, "program"), cmd.program, button,
                                  defaults,
                                  Source{toml::node_view(command_arr.value()[0]), "power_applet"});

        mapLayoutPrimaryButtonCommandArguments(toml::node_view(command_arr.value()[1]), button,
                                               defaults, cmd.arguments, button_index,
                                               extendCfgPath(path_context, "arguments"));

        command = std::move(cmd);
}

bool ConfigMapper::mapLayoutPrimaryButtonData(node_view                       button_data_node,
                                              std::vector<PrimaryButtonData>& buttons,
                                              const std::vector<PrimaryButtonData>& default_buttons,
                                              const PrimaryButtonData&              defaults,
                                              size_t button_index, const QString& path_context) {
        if (button_index > buttons.size()) {
                if (button_index > 1) {
                        QFATAL("The button after %s is too high! Index is %zu",
                               buttons[button_index].label, button_index);
                }

                QFATAL("The button index is too high! %zu", button_index);
        }

        const QString button_path_context = path_context + QString("[%1]").arg(button_index);

        const auto button_table = resolve<toml::table>(button_path_context,
                                                       Source{button_data_node, "power_applet"});
        if (!button_table) {
                buttons = default_buttons;
                return true;
        }

        PrimaryButtonData button{};

        auto enabled = resolve<bool>(path_context,
                                     Source{button_table.value()["enabled"], "power_applet"});
        if (!enabled) { return true; }

        resolveTransformOrDefault<QString>(extendCfgPath(path_context, "id"), button.id, button,
                                           defaults, getPowerButtonIDFromString,
                                           Source{button_table.value()["id"], "power_applet"});

        resolveOrDefault<QString>(extendCfgPath(path_context, "label"), button.label, button,
                                  defaults, Source{button_table.value()["label"], "power_applet"});

        resolveOrDefault<int64_t>(extendCfgPath(path_context, "order"), button.order, button,
                                  defaults, Source{button_table.value()["order"], "power_applet"});

        mapLayoutPrimaryButtonCommand(button_data_node["command"], button, defaults, button.command,
                                      button_index, extendCfgPath(path_context, "command"));

        buttons.insert(buttons.cend(), std::move(button));

        return false;
}

void ConfigMapper::mapLayoutPrimaryButtons(node_view                       primary_buttons_node,
                                           std::vector<PrimaryButtonData>& primary_buttons,
                                           const std::vector<PrimaryButtonData>& defaults,
                                           const QString&                        path_context) {
        const auto buttons = resolve<toml::array>(path_context,
                                                  Source{primary_buttons_node, "power_applet"});
        if (!buttons) {
                primary_buttons = defaults;
                return;
        }

        std::vector<PrimaryButtonData> buttons_found{};

        bool defaulted = false;
        for (size_t i = 0; i != buttons.value().size(); ++i) {
                // TODO Ensure defaults are at least as long as primary_buttons, or pass optional
                //      to avoid adding a button that cannot be defaulted
                defaulted = mapLayoutPrimaryButtonData(node_view(buttons.value().at(i)),
                                                       buttons_found, defaults, defaults[i], i,
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
                                       const LayoutProperties& defaults,
                                       const QString&          path_context) {
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
                                defaults.getPrimaryPowerButtons(),
                                extendCfgPath(path_context, "primary_buttons"));

        layout = std::move(layout_properties);
}

void ConfigMapper::mapEnvironmentProperties(node_view                    environment_node,
                                            EnvironmentProperties&       environment,
                                            const EnvironmentProperties& defaults,
                                            const QString&               path_context) {
        EnvironmentProperties environment_properties{};

        const auto data = resolve<toml::table>(path_context,
                                               Source{environment_node, "power_applet"});
        if (!data) {
                environment_properties = defaults;
                environment            = std::move(environment_properties);
                return;
        }

        // D-Bus mode
        environment_properties.dbus_mode = resolveOr<bool>(extendCfgPath(path_context, "dbus_mode"),
                                                           defaults.getDBusMode(),
                                                           Source{data.value()["dbus_mode"],
                                                                  "power_applet"});

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
        mapWindowProperties(NodePair{config_table["power_applet"]["window"],
                                     config_table["global"]["window"]},
                            config.window_properties, defaults.getWindowProperties(), "window");

        /* Primary Button Properties */
        mapPrimaryButtonProperties(NodePair{config_table["power_applet"]["primary_button"],
                                            config_table["global"]["primary_button"]},
                                   config.primary_button_properties,
                                   defaults.getPrimaryButtonProperties(), "primary_button");
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
                            defaults.getLayoutProperties(), "layout");

        /* Environment Properties */
        mapEnvironmentProperties(config_table["power_applet"]["environment"],
                                 config.environment_properties, defaults.getEnvironmentProperties(),
                                 "environment");
}
