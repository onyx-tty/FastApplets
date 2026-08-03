// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Map.h"

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Schema/Config.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Resolve/Types/Candidate.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

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
T config::map::properties(const config::resolve::Candidates& candidates, const T& defaults,
                          const config::resolve::PathContext& path_context, auto fill_fn) {
        using namespace config;

        std::vector<const toml::table*> resolved = {};

        for (const auto& candidate : candidates.get()) {
                if (const auto* result = resolve::fromAs<toml::table>({candidate}, path_context)) {
                        resolved.push_back(result);
                }
        }

        if (resolved.empty()) { return defaults; }

        auto props = T();
        fill_fn(props, path_context);
        return std::move(props);
}

/* PrimaryButtonParams */

template<applet::type TApplet>
PrimaryButtonParams config::map::primaryButtonParams(
        const config::resolve::Candidates& candidates, const PrimaryButtonParams& defaults,
        const config::resolve::PathContext& path_context) {
        using namespace config;

        const auto* table = resolve::fromAs<toml::table>(candidates, path_context);
        if (!table) { return defaults; }

        PrimaryButtonParams params = {};

        params.per_button = perPrimaryButtonParamsList<TApplet>(
                {candidates.get()[0].makeCopy().withExtension("list").withQuiet(false)},
                defaults.per_button, path_context.makeExtended("list"));

        params.style = primaryButtonStyle(candidates, defaults.style, path_context);

        params.behavior = primaryButtonBehavior(candidates, defaults.behavior, path_context);

        return std::move(params);
}

template<applet::type TApplet>
std::vector<PerPrimaryButtonParams> config::map::perPrimaryButtonParamsList(
        const config::resolve::Candidates&         candidates,
        const std::vector<PerPrimaryButtonParams>& defaults,
        const config::resolve::PathContext&        path_context) {
        using namespace config;

        const auto* arr = resolve::fromAs<toml::array>(candidates, path_context, {.min_size = 1},
                                                       u"Format: [primary buttons...]");
        if (!arr) { return defaults; }

        std::vector<PerPrimaryButtonParams> found = {};
        found.reserve(arr->size());

        for (size_t i = 0; i != arr->size(); ++i) {
                auto new_button =
                        perPrimaryButtonParams<TApplet>(candidates.makeCopy().withExtension(i),
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
std::optional<PerPrimaryButtonParams> config::map::perPrimaryButtonParams(
        const config::resolve::Candidates&  candidates,
        const config::resolve::PathContext& path_context) {
        using namespace config;

        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        const auto* table = resolve::fromAs<toml::table>(candidates, path_context);
        if (!table) { return std::nullopt; }

        PerPrimaryButtonParams new_button = {};

        auto type_str = resolve::from<QString>(candidates.makeCopy().withExtension("id"),
                                               path_context.makeExtended("id"));

        new_button.type = toPrimaryButtonType<TPrimaryButtonType>(type_str.value_or(""));

        if (isNone<TPrimaryButtonType>(new_button.type)) { return std::nullopt; }

        auto t = std::get<TPrimaryButtonType>(new_button.type);

        new_button.text = resolve::from<QString>(candidates.makeCopy().withExtension("text"),
                                                 path_context.makeExtended("text"))
                                  .value_or(textFor(t));

        new_button.command = resolve::from<QString>(candidates.makeCopy().withExtension("command"),
                                                    path_context.makeExtended("command"))
                                     .value_or(commandFor(t));

        new_button.icon = iconFor(t);

        return std::move(new_button);
}

template<applet::type TApplet>
config::schema::Config config::map::config(const toml::table& applet, const toml::table& global,
                                           const config::schema::Config& defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"config.toml";

        auto config = Config();

        Candidates cands = {{.node = node_view(applet), .applet = TApplet, .quiet = true},
                            {.node = node_view(global), .applet = applet::type::global}};

        // TODO: Use enum in .withQuiet() to avoid magic numbers
        config.window_params = windowParams(cands.makeCopy().withExtension("window").withQuiet(false,
                                                                                               0),
                                            defaults.window_params,
                                            PathContext(filename, u"window"));

        config.primary_button_params = primaryButtonParams<TApplet>(
                cands.makeCopy().withExtension("primary_button").withQuiet(false),
                defaults.primary_button_params, PathContext(filename, u"primary_button"));

        return std::move(config);
}
