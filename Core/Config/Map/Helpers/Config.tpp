// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config.h"

#include "Core/Applets/Types/Traits.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/Map/Helpers/Helpers.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Types/Candidates/Candidates.h"
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
        const Candidates& candidates, const PrimaryButtonParams& defaults) {
        return table<PrimaryButtonParams>(candidates, [&defaults, &candidates](
                                                              PrimaryButtonParams& params) {
                params.per_button = perPrimaryButtonParamsList<TApplet>(
                        {candidates[CandidateIndex::Applet][u"list"]}, defaults.per_button);

                params.style = primaryButtonStyle(candidates, defaults.style);

                params.behavior = primaryButtonBehavior(candidates, defaults.behavior);
        }).value_or(defaults);
}

template<applet::Type TApplet>
std::vector<PerPrimaryButtonParams> config::map::helpers::perPrimaryButtonParamsList(
        const Candidates& candidates, const std::vector<PerPrimaryButtonParams>& defaults) {
        using namespace config;

        const auto* arr = resolve::from<toml::array>(candidates,
                {.bounds = ArrayBounds{.min_size = 1}, .format = u"Format: [primary buttons...]"});
        if (!arr) { return defaults; }

        std::vector<PerPrimaryButtonParams> found = {};
        found.reserve(arr->size());

        for (size_t i = 0; i != arr->size(); ++i) {
                auto new_button = perPrimaryButtonParams<TApplet>(candidates[i]);
                if (new_button) { found.push_back(std::move(new_button.value())); }
        }

        if (found.empty()) {
                qWarning() << "No enabled buttons found! Using defaults...";
                return defaults;
        }

        return std::move(found);
}

template<applet::Type TApplet>
std::optional<PerPrimaryButtonParams> config::map::helpers::perPrimaryButtonParams(
        const Candidates& candidates) {
        using namespace config;
        using TPrimaryButtonType = applet::Traits<TApplet>::TPrimaryButtonType;

        // TODO: This has to be reworked so that field() can be applied here
        return table<PerPrimaryButtonParams>(candidates, [&candidates](
                                                                 PerPrimaryButtonParams& params) {
                // Deserializes string representation into TPrimaryButtonType.
                auto type_str = resolve::from<QString>(candidates[u"id"]);
                params.type   = toPrimaryButtonType<TPrimaryButtonType>(type_str.value_or(""));

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
                if (isNone<TPrimaryButtonType>(params.type)) { return; }
                auto type = std::get<TPrimaryButtonType>(params.type);

                PerPrimaryButtonParams defaults = {
                        .text = textFor(type), .command = commandFor(type), .icon = iconFor(type)
                };
                field(params, &PerPrimaryButtonParams::text, candidates, defaults, u"text");
                field(params, &PerPrimaryButtonParams::command, candidates, defaults, u"command");

                params.icon = defaults.icon;
        });
}
