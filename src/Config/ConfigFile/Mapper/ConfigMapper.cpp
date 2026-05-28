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
#include "Config/Types/NodeView.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <initializer_list>
#include <optional>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <utility>
#include <vector>
#include <QDebug>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

template<typename T>
static T mapProperties(const ResolverCandidates& candidates, const T& defaults,
                       const PathContext& path_context, auto fill_fn) {
        std::vector<toml::table> resolved = {};

        for (const auto& candidate : candidates.get()) {
                if (auto result = Resolver::from<toml::table>({candidate}, path_context)) {
                        resolved.push_back(result.value());
                }
        }

        if (resolved.empty()) { return defaults; }

        auto props = T{};
        fill_fn(props, path_context);
        return std::move(props);
}

/* Window Properties */
WindowProperties ConfigMapper::window(const ResolverCandidates& candidates,
                                      const WindowProperties&   defaults,
                                      const PathContext&        path_context) {
        return mapProperties(
                candidates, defaults, path_context,
                [&defaults, &candidates](WindowProperties& window, const PathContext& path_context) {
                        window.size = Resolver::from<QSize>(candidates.makeExtended("size"),
                                                            path_context.makeExtended("size"))
                                              .value_or(defaults.getSize());

                        window.title = Resolver::from<QString>(candidates.makeExtended("title")
                                                                       .makeQuiet(true, 1),
                                                               path_context.makeExtended("title"))
                                               .value_or(defaults.getTitle());
                });
}

/* Primary Button Properties*/
PrimaryButtonProperties ConfigMapper::primaryButton(const ResolverCandidates&      candidates,
                                                    const PrimaryButtonProperties& defaults,
                                                    const PathContext&             path_context) {
        return mapProperties(
                candidates, defaults, path_context,
                [&defaults, &candidates](PrimaryButtonProperties& button,
                                         const PathContext&       path_context) {
                        button.text_alignment = Resolver::from<Qt::Alignment>(
                                                        candidates.makeExtended("text_alignment"),
                                                        path_context.makeExtended("text_alignment"))
                                                        .value_or(defaults.getTextAlignment());

                        button.icon_alignment = Resolver::from<Qt::Alignment>(
                                                        candidates.makeExtended("icon_alignment"),
                                                        path_context.makeExtended("icon_alignment"))
                                                        .value_or(defaults.getIconAlignment());

                        button.icon_size =
                                Resolver::from<QSize>(candidates.makeExtended("icon_size"),
                                                      path_context.makeExtended("icon_size"))
                                        .value_or(defaults.getIconSize());

                        button.policy =
                                Resolver::from<QSizePolicy>(candidates.makeExtended("policy"),
                                                            path_context.makeExtended("policy"))
                                        .value_or(defaults.getPolicy());
                });
}

/* Layout Properties */
LayoutProperties ConfigMapper::layout(const ResolverCandidates& candidates,
                                      const LayoutProperties&   defaults,
                                      const PathContext&        path_context) {
        auto properties = LayoutProperties{};

        const auto data = Resolver::from<toml::table>(candidates, path_context);
        if (!data) { return defaults; }

        // Primary power buttons
        properties.power_buttons = primaryButtons(candidates.makeExtended("primary_buttons"),
                                                  defaults.getPowerButtons(),
                                                  path_context.makeExtended("primary_buttons"));

        return std::move(properties);
}

std::vector<PowerButtonParams> ConfigMapper::primaryButtons(
        const ResolverCandidates& candidates, const std::vector<PowerButtonParams>& defaults,
        const PathContext& path_context) {
        const auto arr = Resolver::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                     "Format: [primary buttons...]");
        if (!arr) { return defaults; }

        std::vector<PowerButtonParams> found = {};

        for (size_t i = 0; i != arr.value().size(); ++i) {
                auto new_button = primaryButton(candidates.makeExtended(i),
                                                path_context.makeExtended(i));
                if (new_button) { found.push_back(std::move(new_button.value())); }
        }

        if (found.empty()) {
                qWarning() << path_context.makePath(applet::power_applet.type)
                                      + ", no enabled buttons found! Using defaults...";
                return defaults;
        }

        return std::move(found);
}

std::optional<PowerButtonParams> ConfigMapper::primaryButton(const ResolverCandidates& candidates,
                                                             const PathContext& path_context) {
        const auto table = Resolver::from<toml::table>(candidates, path_context);
        if (!table) { return {}; }

        PowerButtonParams new_button = {};

        auto type = Resolver::from<QString>(candidates.makeExtended("id"),
                                            path_context.makeExtended("id"));
        if (!type) { return {}; }

        new_button.type = toPowerButtonType(type.value());

        if (new_button.type == power_button_type::none) { return {}; }

        new_button.text = Resolver::from<QString>(candidates.makeExtended("text"),
                                                  path_context.makeExtended("text"))
                                  .value_or(textFor(new_button.type));

        new_button.command = Resolver::from<QString>(candidates.makeExtended("command"),
                                                     path_context.makeExtended("command"))
                                     .value_or(commandFor(new_button.type));

        new_button.icon = iconFor(new_button.type);

        return std::move(new_button);
}
