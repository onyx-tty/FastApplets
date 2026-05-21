// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Resolver/Types/Source.h"
#include "Config/Types/NodePair.h"
#include "Config/Types/NodeView.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

static power_button_type getPowerButtonTypeFromString(const QString& string) {
        static const std::unordered_map<QString, power_button_type> map =
                {{"poweroff", power_button_type::shutdown},
                 {"shutdown", power_button_type::shutdown},
                 {"reboot", power_button_type::reboot},
                 {"suspend", power_button_type::suspend},
                 {"hibernate", power_button_type::hibernate}};

        // TODO Replace with map.find()
        if (!map.contains(string)) { return power_button_type::none; }

        return map.at(string);
}

template<typename T>
static T mapProperties(NodePair nodes, const T& defaults, const PathContext& path_context,
                       auto fill_fn) {
        // Resolve power_data and global_data
        auto power_data = Resolver::from<toml::table>({Source{.node  = nodes.primary,
                                                              .scope = applet::power_applet.scope,
                                                              .quiet = true}},
                                                      path_context);

        auto global_data = Resolver::from<toml::table>({Source{.node  = nodes.fallback,
                                                               .scope = applet::global.scope}},
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
        window = mapProperties(nodes, defaults, path_context,
                               [&defaults](NodePair nodes, WindowProperties& window,
                                           const PathContext& path_context) {
                                       window.size = Resolver::fromOr<QSize>(
                                               {Source{.node  = nodes.primary["size"],
                                                       .scope = applet::power_applet.scope},
                                                Source{.node  = nodes.fallback["size"],
                                                       .scope = applet::global.scope}},
                                               defaults.getSize(),
                                               path_context.getExtended("size"));

                                       window.title = Resolver::fromOr<QString>(
                                               {Source{.node  = nodes.primary["title"],
                                                       .scope = applet::power_applet.scope},
                                                Source{.node  = nodes.fallback["title"],
                                                       .scope = applet::global.scope}},
                                               defaults.getTitle(),
                                               path_context.getExtended("title"));
                               });
}

/* Primary Button Properties*/
void ConfigMapper::mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                    const PrimaryButtonProperties& defaults,
                                    const PathContext&             path_context) {
        button = mapProperties(nodes, defaults, path_context,
                               [&defaults](NodePair nodes, PrimaryButtonProperties& button,
                                           const PathContext& path_context) {
                                       button.text_alignment = Resolver::fromOr<Qt::Alignment>(
                                               {Source{.node  = nodes.primary["text_alignment"],
                                                       .scope = applet::power_applet.scope},
                                                Source{.node  = nodes.fallback["text_alignment"],
                                                       .scope = applet::global.scope}},
                                               defaults.getTextAlignment(),
                                               path_context.getExtended("text_alignment"));

                                       button.icon_alignment = Resolver::fromOr<Qt::Alignment>(
                                               {Source{.node  = nodes.primary["icon_alignment"],
                                                       .scope = applet::power_applet.scope},
                                                Source{.node  = nodes.fallback["icon_alignment"],
                                                       .scope = applet::global.scope}},
                                               defaults.getIconAlignment(),
                                               path_context.getExtended("icon_alignment"));

                                       button.icon_size = Resolver::fromOr<QSize>(
                                               {Source{.node  = nodes.primary["icon_size"],
                                                       .scope = applet::power_applet.scope},
                                                Source{.node  = nodes.fallback["icon_size"],
                                                       .scope = applet::global.scope}},
                                               defaults.getIconSize(),
                                               path_context.getExtended("icon_size"));

                                       button.policy = Resolver::fromOr<QSizePolicy>(
                                               {Source{.node  = nodes.primary["policy"],
                                                       .scope = applet::power_applet.scope},
                                                Source{.node  = nodes.fallback["policy"],
                                                       .scope = applet::global.scope}},
                                               defaults.getPolicy(),
                                               path_context.getExtended("policy"));
                               });
}

/* Layout Properties */
void ConfigMapper::mapLayout(node_view layout_node, LayoutProperties& layout,
                             const LayoutProperties& defaults, const PathContext& path_context) {
        LayoutProperties layout_properties{};

        const auto data = Resolver::from<toml::table>({Source{.node  = layout_node,
                                                              .scope = applet::power_applet.scope}},
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
        const auto buttons =
                Resolver::from<toml::array>({Source{.node  = primary_buttons_node,
                                                    .scope = applet::power_applet.scope}},
                                            path_context, {.min_size = 1},
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
        const auto button_table =
                Resolver::from<toml::table>({Source{.node  = button_params_node,
                                                    .scope = applet::power_applet.scope}},
                                            path_context);
        if (!button_table) {
                buttons = defaults;
                return true;
        }

        PowerButtonParams button{};

        auto enabled = Resolver::from<bool>({Source{.node  = button_table.value()["enabled"],
                                                    .scope = applet::power_applet.scope}},
                                            path_context.getExtended("enabled"));
        if (!enabled || !enabled.value()) { return false; }

        auto id_result = Resolver::from<QString>({Source{.node  = button_table.value()["id"],
                                                         .scope = applet::power_applet.scope}},
                                                 path_context.getExtended("id"));
        if (!id_result) {
                buttons = defaults;
                return true;
        }
        button.id = getPowerButtonTypeFromString(id_result.value());

        auto text_result = Resolver::from<QString>({Source{.node  = button_table.value()["text"],
                                                           .scope = applet::power_applet.scope}},
                                                   path_context.getExtended("text"));
        if (!text_result) {
                buttons = defaults;
                return true;
        }
        button.text = text_result.value();

        auto order_result = Resolver::from<int64_t>({Source{.node  = button_table.value()["order"],
                                                            .scope = applet::power_applet.scope}},
                                                    path_context.getExtended("order"));
        if (!order_result) {
                buttons = defaults;
                return true;
        }
        button.order = order_result.value();

        mapCommand(button_params_node["command"], buttons, defaults, button.command,
                   path_context.getExtended("command"));

        button.icon = iconFor(button.id);

        buttons.insert(buttons.cend(), std::move(button));

        return false;
}

void ConfigMapper::mapCommand(node_view command_node, std::vector<PowerButtonParams>& buttons,
                              const std::vector<PowerButtonParams>& defaults, QString& command,
                              const PathContext& path_context) {
        auto command_raw = Resolver::from<QString>({Source{.node  = command_node,
                                                           .scope = applet::power_applet.scope}},
                                                   path_context);
        if (!command_raw) {
                buttons = defaults;
                return;
        }

        command = std::move(command_raw.value());
}
