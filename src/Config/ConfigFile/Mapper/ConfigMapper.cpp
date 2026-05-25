// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"
#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Resolver/Types/ResolverCandidate.h"
#include "Config/Types/NodePair.h"
#include "Config/Types/NodeView.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <utility>
#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

template<typename T>
static T mapProperties(NodePair nodes, const T& defaults, const PathContext& path_context,
                       auto fill_fn) {
        auto power =
                Resolver::from<toml::table>({ResolverCandidate{.node  = nodes.primary,
                                                               .scope = applet::power_applet.scope,
                                                               .quiet = true}},
                                            path_context);

        auto global = Resolver::from<toml::table>({ResolverCandidate{.node = nodes.fallback,
                                                                     .scope = applet::global.scope}},
                                                  path_context);

        // Use hardcoded defaults if no tables found
        if (!power && !global) { return defaults; }

        T props{};
        fill_fn(nodes, props, path_context);
        return std::move(props);
}

/* Window Properties */
WindowProperties ConfigMapper::window(NodePair nodes, const WindowProperties& defaults,
                                      const PathContext& path_context) {
        return mapProperties(
                nodes, defaults, path_context,
                [&defaults](NodePair nodes, WindowProperties& window,
                            const PathContext& path_context) {
                        window.size = Resolver::from<QSize>(
                                              {ResolverCandidate{.node = nodes.primary["size"],
                                                                 .scope = applet::power_applet.scope},
                                               ResolverCandidate{.node  = nodes.fallback["size"],
                                                                 .scope = applet::global.scope}},
                                              path_context.getExtended("size"))
                                              .value_or(defaults.getSize());

                        window.title =
                                Resolver::from<QString>(
                                        {ResolverCandidate{.node  = nodes.primary["title"],
                                                           .scope = applet::power_applet.scope},
                                         ResolverCandidate{.node  = nodes.fallback["title"],
                                                           .scope = applet::global.scope}},
                                        path_context.getExtended("title"))
                                        .value_or(defaults.getTitle());
                });
}

/* Primary Button Properties*/
PrimaryButtonProperties ConfigMapper::primaryButton(NodePair                       nodes,
                                                    const PrimaryButtonProperties& defaults,
                                                    const PathContext&             path_context) {
        return mapProperties(
                nodes, defaults, path_context,
                [&defaults](NodePair nodes, PrimaryButtonProperties& button,
                            const PathContext& path_context) {
                        button.text_alignment =
                                Resolver::from<Qt::Alignment>(
                                        {ResolverCandidate{.node  = nodes.primary["text_alignment"],
                                                           .scope = applet::power_applet.scope},
                                         ResolverCandidate{.node = nodes.fallback["text_alignment"],
                                                           .scope = applet::global.scope}},
                                        path_context.getExtended("text_alignment"))
                                        .value_or(defaults.getTextAlignment());

                        button.icon_alignment =
                                Resolver::from<Qt::Alignment>(
                                        {ResolverCandidate{.node  = nodes.primary["icon_alignment"],
                                                           .scope = applet::power_applet.scope},
                                         ResolverCandidate{.node = nodes.fallback["icon_alignment"],
                                                           .scope = applet::global.scope}},
                                        path_context.getExtended("icon_alignment"))
                                        .value_or(defaults.getIconAlignment());

                        button.icon_size =
                                Resolver::from<QSize>(
                                        {ResolverCandidate{.node  = nodes.primary["icon_size"],
                                                           .scope = applet::power_applet.scope},
                                         ResolverCandidate{.node  = nodes.fallback["icon_size"],
                                                           .scope = applet::global.scope}},
                                        path_context.getExtended("icon_size"))
                                        .value_or(defaults.getIconSize());

                        button.policy =
                                Resolver::from<QSizePolicy>(
                                        {ResolverCandidate{.node  = nodes.primary["policy"],
                                                           .scope = applet::power_applet.scope},
                                         ResolverCandidate{.node  = nodes.fallback["policy"],
                                                           .scope = applet::global.scope}},
                                        path_context.getExtended("policy"))
                                        .value_or(defaults.getPolicy());
                });
}

/* Layout Properties */
LayoutProperties ConfigMapper::layout(node_view node, const LayoutProperties& defaults,
                                      const PathContext& path_context) {
        LayoutProperties properties{};

        const auto data =
                Resolver::from<toml::table>({ResolverCandidate{.node = node,
                                                               .scope = applet::power_applet.scope}},
                                            path_context);
        if (!data) { return defaults; }

        // Primary power buttons
        properties.power_buttons = primaryButtons(data.value()["primary_buttons"],
                                                  defaults.getPowerButtons(),
                                                  path_context.getExtended("primary_buttons"));

        return std::move(properties);
}

std::vector<PowerButtonParams> ConfigMapper::primaryButtons(
        node_view node, const std::vector<PowerButtonParams>& defaults,
        const PathContext& path_context) {
        const auto arr = Resolver::from<toml::array>(
                {ResolverCandidate{.node = node, .scope = applet::power_applet.scope}},
                path_context, {.min_size = 1}, "Format: [primary buttons...]");
        if (!arr) { return defaults; }

        std::vector<PowerButtonParams> found{};

        for (size_t i = 0; i != arr.value().size(); ++i) {
                auto new_button = primaryButton(node_view(arr.value().at(i)),
                                                path_context.getExtended(i));
                if (new_button) { found.push_back(std::move(new_button.value())); }
        }

        if (found.empty()) {
                QWARNING() << path_context.makePath(applet::power_applet.scope)
                                      + ", no enabled buttons found! Using defaults...";
                return defaults;
        }

        return std::move(found);
}

std::optional<PowerButtonParams> ConfigMapper::primaryButton(node_view          node,
                                                             const PathContext& path_context) {
        const auto table =
                Resolver::from<toml::table>({ResolverCandidate{.node = node,
                                                               .scope = applet::power_applet.scope}},
                                            path_context);
        if (!table) { return {}; }

        PowerButtonParams new_button{};

        auto type = Resolver::from<QString>({ResolverCandidate{.node = table.value()["id"],
                                                               .scope = applet::power_applet.scope}},
                                            path_context.getExtended("id"));
        if (!type) { return {}; }

        new_button.type = toPowerButtonType(type.value());

        if (new_button.type == power_button_type::none) { return {}; }

        new_button.text =
                Resolver::from<QString>({ResolverCandidate{.node  = table.value()["text"],
                                                           .scope = applet::power_applet.scope}},
                                        path_context.getExtended("text"))
                        .value_or(textFor(new_button.type));

        new_button.command =
                Resolver::from<QString>({ResolverCandidate{.node  = table.value()["command"],
                                                           .scope = applet::power_applet.scope}},
                                        path_context.getExtended("command"))
                        .value_or(commandFor(new_button.type));

        new_button.icon = iconFor(new_button.type);

        return std::move(new_button);
}
