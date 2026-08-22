// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config.h"

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/View/View.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PerPrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonParams.h"

#include <QDebug>
#include <QtGlobal>
#include <TomlQt/ArrayBounds.h>
#include <optional>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>

class QString;

template<applet::Type TApplet>
PrimaryButtonParams config::map::helpers::primaryButtonParams(
        const ConfigView& node, const PrimaryButtonParams& defaults) {
        return {.per_button = perPrimaryButtonParamsList<TApplet>(node["list"], defaults.per_button),
                .style    = primaryButtonStyle(node, defaults.style),
                .behavior = primaryButtonBehavior(node, defaults.behavior)};
}

template<applet::Type TApplet>
std::vector<PerPrimaryButtonParams> config::map::helpers::perPrimaryButtonParamsList(
        const ConfigView& node, const std::vector<PerPrimaryButtonParams>& defaults) {
        using namespace config;

        ArrayBounds bounds = {.min_size = 1};

        const auto* arr = node.resolve<toml::array>(bounds);
        if (!arr) { return defaults; }

        std::vector<PerPrimaryButtonParams> found = {};
        found.reserve(arr->size());

        for (int i = 0; i != arr->size(); ++i) {
                auto new_button = perPrimaryButtonParams<TApplet>(node[i]);
                if (new_button) { found.push_back(std::move(new_button.value())); }
        }

        if (found.empty()) {
                qWarning() << "No buttons found";
                return defaults;
        }

        return std::move(found);
}

template<applet::Type TApplet>
std::optional<PerPrimaryButtonParams> config::map::helpers::perPrimaryButtonParams(
        const ConfigView& node) {
        using namespace config;
        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        PerPrimaryButtonParams params = {};

        auto type_str = node["id"].resolve<QString>().value_or("");
        params.type   = toPrimaryButtonType<TPrimaryButtonType>(type_str);

        // If provided type is missing or invalid then it's impossible to deduce defaults.
        // As a result, this function has to return std::nullopt to indicate that
        // PerPrimaryButtonParams needs to be defaulted.
        //
        // If validation took place in separation and before extraction, it'd be possible
        // to know if defaulting will have to happen at all, and thus the function
        // could progress in such case. But with current architecture, this is not
        // possible. As a result, the function has to return regardless. Type must be
        // valid at all times.
        //
        // It is worth mentioning that isNone returns false for std::monostate, and
        // textFor, commandFor, and iconFor can handle std::monostate themselves.
        if (isNone<TPrimaryButtonType>(params.type)) { return std::nullopt; }
        auto type = std::get<TPrimaryButtonType>(params.type);

        PerPrimaryButtonParams defaults = {
                .text = textFor(type), .command = commandFor(type), .icon = iconFor(type)
        };

        params.text    = node["text"].resolve<QString>().value_or(std::move(defaults.text));
        params.command = node["command"].resolve<QString>().value_or(std::move(defaults.command));
        params.icon    = std::move(defaults.icon);

        return std::move(params);
}
