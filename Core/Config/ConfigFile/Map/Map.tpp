// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Map.h"

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Schema/Config.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Select/PathContext/PathContext.h"
#include "Core/Config/Select/Types/Candidates.h"
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
T config::map::properties(const config::select::Candidates& candidates, const T& defaults,
                          auto fill_fn) {
        using namespace config;

        std::vector<const toml::table*> resolved = {};

        for (const auto& candidate : candidates.get()) {
                if (const auto* result = resolve::from<toml::table>({candidate})) {
                        resolved.push_back(result);
                }
        }

        if (resolved.empty()) { return defaults; }

        auto props = T();
        fill_fn(props);
        return std::move(props);
}

/* PrimaryButtonParams */

template<applet::Type TApplet>
PrimaryButtonParams config::map::primaryButtonParams(const config::select::Candidates& candidates,
                                                     const PrimaryButtonParams&        defaults) {
        using namespace config;
        using config::select::CandidateIndex;

        const auto* table = resolve::from<toml::table>(candidates);
        if (!table) { return defaults; }

        PrimaryButtonParams params = {};

        params.per_button = perPrimaryButtonParamsList<TApplet>({candidates[CandidateIndex::Applet]
                                                                         .makeCopy()
                                                                         .withExtension(u"list")
                                                                         .withQuiet(false)},
                                                                defaults.per_button);

        params.style = primaryButtonStyle(candidates, defaults.style);

        params.behavior = primaryButtonBehavior(candidates, defaults.behavior);

        return std::move(params);
}

template<applet::Type TApplet>
std::vector<PerPrimaryButtonParams> config::map::perPrimaryButtonParamsList(
        const config::select::Candidates&          candidates,
        const std::vector<PerPrimaryButtonParams>& defaults) {
        using namespace config;

        const auto* arr = resolve::from<toml::array>(candidates, {.min_size = 1},
                                                     u"Format: [primary buttons...]");
        if (!arr) { return defaults; }

        std::vector<PerPrimaryButtonParams> found = {};
        found.reserve(arr->size());

        for (size_t i = 0; i != arr->size(); ++i) {
                auto new_button = perPrimaryButtonParams<TApplet>(
                        candidates.makeCopy().withExtension(i));
                if (new_button) { found.push_back(std::move(new_button.value())); }
        }

        if (found.empty()) {
                qWarning() << "No enabled buttons found! Using defaults...";
                return defaults;
        }

        return std::move(found);
}

template<applet::Type TApplet>
std::optional<PerPrimaryButtonParams> config::map::perPrimaryButtonParams(
        const config::select::Candidates& candidates) {
        using namespace config;
        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        const auto* table = resolve::from<toml::table>(candidates);
        if (!table) { return std::nullopt; }

        PerPrimaryButtonParams new_button = {};

        auto type_str = resolve::from<QString>(candidates.makeCopy().withExtension(u"id"));

        new_button.type = toPrimaryButtonType<TPrimaryButtonType>(type_str.value_or(""));

        if (isNone<TPrimaryButtonType>(new_button.type)) { return std::nullopt; }

        auto t = std::get<TPrimaryButtonType>(new_button.type);

        new_button.text = resolve::from<QString>(candidates.makeCopy().withExtension(u"text"))
                                  .value_or(textFor(t));

        new_button.command = resolve::from<QString>(candidates.makeCopy().withExtension(u"command"))
                                     .value_or(commandFor(t));

        new_button.icon = iconFor(t);

        return std::move(new_button);
}

template<applet::Type TApplet>
config::schema::Config config::map::config(const toml::table& applet, const toml::table& global,
                                           const config::schema::Config& defaults) {
        using config::select::CandidateIndex;
        using config::select::PathContext;

        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"config.toml";

        auto config = Config();

        Candidates cands = {{.node         = node_view(applet),
                             .applet       = TApplet,
                             .quiet        = true,
                             .path_context = PathContext(filename, u"")},
                            {.node         = node_view(global),
                             .applet       = applet::Type::Global,
                             .path_context = PathContext(filename, u"")}};

        config.window_params = windowParams(cands.makeCopy()
                                                    .withExtension(u"window")
                                                    .withQuiet(CandidateIndex::Applet, false),
                                            defaults.window_params);

        config.primary_button_params = primaryButtonParams<TApplet>(
                cands.makeCopy().withExtension(u"primary_button").withQuiet(false),
                defaults.primary_button_params);

        return std::move(config);
}
