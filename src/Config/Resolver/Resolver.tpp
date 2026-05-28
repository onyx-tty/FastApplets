// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/Types/ResolverCandidate.h"
#include "Config/Types/NodeView.h"
#include "PathContext/PathContext.h"
#include "Resolver.h"
#include "TomlQt/ArrayBounds.h"
#include "TomlQt/TomlQt.h"

#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>
#include <type_traits>
#include <utility>
#include <QDebug>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

template<typename T>
std::optional<T> Resolver::from(const ResolverCandidates&  candidates,
                                const PathContext&         path_context,
                                const tomlqt::ArrayBounds& arr_bounds, const QString& arr_format) {
        using DT = std::decay_t<T>;

        // Convert pointers to std::optional
        // TODO Eliminate this lambda, toml::array and toml::table should be pointers
        //      But as long as the return type of resolve is stuck as std::optional,
        //      this cannot be done.
        static auto normalize = [&](auto&& raw) -> std::optional<T> {
                using R = std::decay_t<decltype(raw)>;
                if constexpr (std::is_same_v<R, std::optional<DT>>) {
                        return raw;
                } else if constexpr (std::is_pointer_v<R>) {
                        return raw ? std::optional<DT>(*raw) : std::nullopt;
                } else {
                        static_assert(false, "Unknown accessor return type!");
                }
        };

        // Collapse extraction logic into that of a corresponding type
        static auto extract = [&](node_view                  node,
                                  const tomlqt::ArrayBounds& arr_bounds = {}) -> std::optional<DT> {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        return normalize(node.as_table());
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        // TODO Extract as a separate function
                        using result = tomlqt::ArrayBounds::validation_result;

                        std::optional<toml::array> arr = normalize(node.as_array());
                        if (!arr) { return std::nullopt; }

                        auto res = arr_bounds.validate(arr.value());
                        if (res == result::min_size_fail) {
                                qWarning()
                                        << QString("arr size < min_size! min_size: %1, arr size: %2")
                                                   .arg(QString::number(arr_bounds.min_size.value()),
                                                        QString::number(arr->size()));
                                return std::nullopt;
                        }

                        if (res == result::max_size_fail) {
                                qWarning()
                                        << QString("arr size > max_size! max_size: %1, arr size: %2")
                                                   .arg(QString::number(arr_bounds.max_size.value()),
                                                        QString::number(arr->size()));
                                return std::nullopt;
                        }
                        return std::move(arr);
                } else if constexpr (std::is_same_v<DT, QSize>) {
                        return normalize(tomlqt::tryGetQSize(node));
                } else if constexpr (std::is_same_v<DT, Qt::Alignment>) {
                        return normalize(tomlqt::tryGetQtAlignment(node));
                } else if constexpr (std::is_same_v<DT, QSizePolicy>) {
                        return normalize(tomlqt::tryGetQSizePolicy(node));
                } else if constexpr (std::is_same_v<DT, QString>) {
                        return normalize(tomlqt::tryGetQString(node));
                } else {
                        return normalize(node.value<DT>());
                }
        };

        // Collapse logging message variants
        static auto log = [&](const QString& path) {
                if constexpr (std::is_same_v<DT, toml::table>) {
                        qWarning()
                                << QString("%1, missing or wrong type! Using defaults...").arg(path);
                } else if constexpr (std::is_same_v<DT, toml::array>) {
                        qWarning()
                                << QString("%1, missing or wrong type! Format: %2. Using defaults...")
                                           .arg(path, QString("must be an array! Format: %1")
                                                              .arg(arr_format));
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

                auto result = extract(candidate.node, arr_bounds);
                if (!result) {
                        if (!silence_logs) { log(path_context.makePath(candidate.applet)); }
                        continue;
                }

                qDebug() << path_context.makePath(candidate.applet) << "found!";
                return *result;
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}

template<typename TAttribute, typename TObject>
void Resolver::fromOrDefault(const ResolverCandidates& candidates, TAttribute& attribute,
                             TObject& object, const TObject& object_defaults,
                             const PathContext& path_context, const tomlqt::ArrayBounds& arr_bounds,
                             const QString& arr_format) {
        if (auto result = from<TAttribute>(candidates, path_context, arr_bounds, arr_format)) {
                attribute = result.value();
        } else {
                object = object_defaults;
        }
}

template<typename TRaw, typename TAttribute, typename TObject, typename Transform>
void Resolver::fromTransformOrDefault(const ResolverCandidates& candidates, TAttribute& attribute,
                                      TObject& object, const TObject& object_defaults,
                                      Transform&& transform, const PathContext& path_context) {
        if (auto result = from<TRaw>(candidates, path_context)) {
                attribute = transform(std::move(result.value()));
        } else {
                object = object_defaults;
        }
}
