// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ConfigMapper.h"

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Resolve/Types/ResolverCandidate.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/LayoutProperties.h"
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
T config::ConfigMapper::mapProperties(const Candidates& candidates, const T& defaults,
                                      const PathContext& path_context, auto fill_fn) {
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

/* LayoutProperties */

template<applet::type TApplet>
LayoutProperties config::ConfigMapper::layoutProperties(const Candidates&       candidates,
                                                        const LayoutProperties& defaults,
                                                        const PathContext&      path_context) {
        using namespace config;

        auto layout = LayoutProperties{};

        const auto* data = resolve::fromAs<toml::table>(candidates, path_context);
        if (!data) { return defaults; }

        layout.primary_buttons =
                primaryButtonParams<TApplet>(candidates.makeCopy().withExtension("primary_buttons"),
                                             defaults.primary_buttons,
                                             path_context.makeExtended("primary_buttons"));

        return std::move(layout);
}

template<applet::type TApplet>
std::vector<PrimaryButtonParams> config::ConfigMapper::primaryButtonParams(
        const Candidates& candidates, const std::vector<PrimaryButtonParams>& defaults,
        const PathContext& path_context) {
        using namespace config;

        const auto* arr = resolve::fromAs<toml::array>(candidates, path_context, {.min_size = 1},
                                                       u"Format: [primary buttons...]");
        if (!arr) { return defaults; }

        std::vector<PrimaryButtonParams> found = {};

        for (size_t i = 0; i != arr->size(); ++i) {
                auto new_button = primaryButtonParams<TApplet>(candidates.makeCopy().withExtension(
                                                                       i),
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
std::optional<PrimaryButtonParams> config::ConfigMapper::primaryButtonParams(
        const Candidates& candidates, const PathContext& path_context) {
        using namespace config;

        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        const auto* table = resolve::fromAs<toml::table>(candidates, path_context);
        if (!table) { return std::nullopt; }

        PrimaryButtonParams new_button = {};

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

        new_button.icon = std::move(iconFor(t));

        return std::move(new_button);
}

template<applet::type TApplet>
config::schema::Config config::ConfigMapper::config(const toml::table& applet,
                                                    const toml::table& global,
                                                    const Config&      defaults) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { qFatal("QApplication has not been instantiated yet!"); }

        constexpr QStringView filename = u"config.toml";

        auto config = Config();

        Candidates cands = {{.node = node_view(applet), .applet = TApplet, .quiet = true},
                            {.node = node_view(global), .applet = applet::type::global}};

        // TODO: Use enum in .withQuiet() to avoid magic numbers
        config.window_params = windowParams(cands.makeCopy().withExtension("window").withQuiet(false,
                                                                                               0),
                                            defaults.getWindowParams(),
                                            PathContext(filename, u"window"));

        config.primary_button_style = primaryButtonStyle(cands.makeCopy().withExtension(
                                                                 "primary_button"),
                                                         defaults.getPrimaryButtonStyle(),
                                                         PathContext(filename, u"primary_button"));

        config.layout_properties = layoutProperties<TApplet>(
                {cands.get()[0].makeCopy().withExtension("layout").withQuiet(false)},
                defaults.getLayoutProperties(), PathContext(filename, u"layout"));

        return std::move(config);
}
