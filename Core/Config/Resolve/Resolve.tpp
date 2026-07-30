// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Resolve/Types/ResolverCandidate.h"
#include "Core/Config/Types/NodeView.h"
#include "PathContext/PathContext.h"
#include "Resolve.h"

#include <TomlQt/TomlQt.h>
#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <utility>
#include <QDebug>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStringView>
#include <Qt>
#include <QtGlobal>

template<typename T>
requires(!std::is_same_v<T, toml::table> && !std::is_same_v<T, toml::array>)
std::optional<T> config::resolve::from(const Candidates&  candidates,
                                       const PathContext& path_context) {
        using DT = std::decay_t<T>;

        // Collapse extraction logic into that of a corresponding type
        static auto extract = [&](node_view node) -> std::optional<DT> {
                return tomlqt::value<DT>(node);
        };

        // Collapse logging message variants
        static auto log = [&](QStringView path) {
                if (path.isNull()) { qFatal("Passed null path!"); }

                qWarning() << QString("%1, missing or wrong type! Using defaults...").arg(path);
        };

        // Validate and attempt extraction of each passed candidate, prioritizing earliest ones
        const auto candidate_ptr = candidates.get().begin();
        for (size_t i = 0; i != candidates.get().size(); ++i) {
                const auto& candidate = candidate_ptr[i];

                // If 'i' is not the last index, then candidates[i] is an override
                // TODO If primary candidate is valid and fallback is not then quiet on
                //      primary candidate will result in no logs being printed at all
                //      Iteration over candidates should probably be done in reverse to
                //      track if fallback is missing, and if it is then quiet should
                //      likely be ignored.
                bool is_override  = (i != candidates.get().size() - 1);
                // If override or explicitly marked "quiet", don't log anything
                bool silence_logs = is_override || candidate.quiet;

                auto res = extract(candidate.node);
                if (!res) {
                        if (!silence_logs) { log(path_context.makePath(candidate.applet)); }
                        continue;
                }

                qDebug() << path_context.makePath(candidate.applet) << "found!";
                return *res;
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}

template<typename T>
requires(!std::is_same_v<std::decay_t<T>, QSize> && !std::is_same_v<std::decay_t<T>, Qt::Alignment>
         && !std::is_same_v<std::decay_t<T>, QSizePolicy>
         && !std::is_same_v<std::decay_t<T>, QString>)
const T* config::resolve::fromAs(const Candidates& candidates, const PathContext& path_context,
                                 const ArrayBounds& arr_bounds, QStringView arr_format) {
        using DT = const std::decay_t<T>;

        // Collapse extraction logic into that of a corresponding type
        static auto extract = [&](node_view node, const ArrayBounds& arr_bounds = {}) -> DT* {
                if constexpr (std::is_same_v<DT, toml::array>) {
                        return tomlqt::asArrayWithBounds(node, arr_bounds);
                } else {
                        return node.as<DT>();
                }
        };

        // Collapse logging message variants
        static auto log = [&, arr_format](QStringView path) {
                if (path.isNull()) { qFatal("Passed null path!"); }

                if constexpr (std::is_same_v<DT, toml::array>) {
                        qWarning()
                                << QString("%1, missing or wrong type! Format: %2. Using defaults...")
                                           .arg(path, arr_format.toString());
                } else {
                        qWarning()
                                << QString("%1, missing or wrong type! Using defaults...").arg(path);
                }
        };

        // Validate and attempt extraction of each passed candidate, prioritizing earliest ones
        const auto candidate_ptr = candidates.get().begin();
        for (size_t i = 0; i != candidates.get().size(); ++i) {
                const auto& candidate = candidate_ptr[i];

                // If 'i' is not the last index, then candidates[i] is an override
                // TODO If primary candidate is valid and fallback is not then quiet on
                //      primary candidate will result in no logs being printed at all
                //      Iteration over candidates should probably be done in reverse to
                //      track if fallback is missing, and if it is then quiet should
                //      likely be ignored.
                bool is_override  = (i != candidates.get().size() - 1);
                // If override or explicitly marked "quiet", don't log anything
                bool silence_logs = is_override || candidate.quiet;

                auto* res = extract(candidate.node, arr_bounds);
                if (!res) {
                        if (!silence_logs) { log(path_context.makePath(candidate.applet)); }
                        continue;
                }

                qDebug() << path_context.makePath(candidate.applet) << "found!";
                return res;
        }

        // Use hardcoded defaults if extraction failed
        return nullptr;
}

template<typename TAttribute, typename TObject>
void config::resolve::fromOrDefault(const Candidates& candidates, TAttribute& attribute,
                                    TObject& object, const TObject& object_defaults,
                                    const PathContext& path_context, const ArrayBounds& arr_bounds,
                                    QStringView arr_format) {
        if (auto res = from<TAttribute>(candidates, path_context, arr_bounds, arr_format)) {
                attribute = std::move(res.value());
        } else {
                object = object_defaults;
        }
}

template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
void config::resolve::fromTransformOrDefault(const Candidates& candidates, TAttribute& attribute,
                                             TObject& object, const TObject& object_defaults,
                                             Transform transform, const PathContext& path_context) {
        if (auto res = from<TRaw>(candidates, path_context)) {
                attribute = transform(std::move(res.value()));
        } else {
                object = object_defaults;
        }
}
