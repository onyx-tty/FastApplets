// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigMapper.h"

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/Resolver/PathContext/PathContext.h"
#include "Core/Config/Resolver/Resolver.h"
#include "Core/Config/Resolver/Types/ResolverCandidate.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButtonParams.h"

#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<typename T>
T ConfigMapper::mapProperties(const ResolverCandidates& candidates, const T& defaults,
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

/* Layout Properties */

template<applet::type TApplet>
LayoutProperties ConfigMapper::layout(const ResolverCandidates& candidates,
                                      const LayoutProperties&   defaults,
                                      const PathContext&        path_context) {
        auto properties = LayoutProperties{};

        const auto data = Resolver::from<toml::table>(candidates, path_context);
        if (!data) { return defaults; }

        properties.primary_buttons =
                primaryButtons<TApplet>(candidates.makeExtended("primary_buttons"),
                                        defaults.getPrimaryButtons(),
                                        path_context.makeExtended("primary_buttons"));

        return std::move(properties);
}

template<applet::type TApplet>
std::vector<PrimaryButtonParams> ConfigMapper::primaryButtons(
        const ResolverCandidates& candidates, const std::vector<PrimaryButtonParams>& defaults,
        const PathContext& path_context) {
        const auto arr = Resolver::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                     u"Format: [primary buttons...]");
        if (!arr) { return defaults; }

        std::vector<PrimaryButtonParams> found = {};

        for (size_t i = 0; i != arr.value().size(); ++i) {
                auto new_button = primaryButton<TApplet>(candidates.makeExtended(i),
                                                         path_context.makeExtended(i));
                if (new_button) { found.push_back(std::move(new_button.value())); }
        }

        if (found.empty()) {
                qWarning() << "No enabled buttons found! Using defaults...";
                return defaults;
        }

        return std::move(found);
}

template<applet::type TApplet>
std::optional<PrimaryButtonParams> ConfigMapper::primaryButton(const ResolverCandidates& candidates,
                                                               const PathContext& path_context) {
        using TPrimaryButtonType = AppletTraits<TApplet>::TPrimaryButtonType;

        const auto table = Resolver::from<toml::table>(candidates, path_context);
        if (!table) { return std::nullopt; }

        PrimaryButtonParams new_button = {};

        auto type_str = Resolver::from<QString>(candidates.makeExtended("id"),
                                                path_context.makeExtended("id"));

        new_button.type = toPrimaryButtonType<TPrimaryButtonType>(type_str.value_or(""));

        if (isNone<TPrimaryButtonType>(new_button.type)) { return std::nullopt; }

        auto t = std::get<TPrimaryButtonType>(new_button.type);

        new_button.text = Resolver::from<QString>(candidates.makeExtended("text"),
                                                  path_context.makeExtended("text"))
                                  .value_or(textFor(t));

        new_button.command = Resolver::from<QString>(candidates.makeExtended("command"),
                                                     path_context.makeExtended("command"))
                                     .value_or(commandFor(t));

        // QProcess::splitCommand treats single quotes as regular characters, breaking
        // shell commands that rely on them.
        // To get them to work as they should, reinterpret single quotes as double quotes.
        new_button.command.replace("'", "\"");

        new_button.icon = std::move(iconFor(t));

        return std::move(new_button);
}

template<applet::type TApplet>
Config ConfigMapper::config(const toml::table& applet, const toml::table& global,
                            const Config& defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"config.toml";

        Config config = Config{};

        ResolverCandidates cands = {{.node = node_view(applet), .applet = TApplet, .quiet = true},
                                    {.node = node_view(global), .applet = applet::type::global}};

        /* Window Properties */
        // TODO: Use enum in .makeQuiet() to avoid magic numbers
        config.window_properties = window(cands.makeExtended("window").makeQuiet(false, 0),
                                          defaults.getWindowProperties(),
                                          PathContext{filename, u"window"});

        /* Primary Button Properties */
        config.primary_button_properties = primaryButton(cands.makeExtended("primary_button"),
                                                         defaults.getPrimaryButtonProperties(),
                                                         PathContext{filename, u"primary_button"});

        /* Layout Properties */
        config.layout_properties = layout<TApplet>({cands.get()[0].makeExtended("layout").makeQuiet(
                                                           false)},
                                                   defaults.getLayoutProperties(),
                                                   PathContext{filename, u"layout"});

        return std::move(config);
}
