// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigMapper.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/Config/Resolver/PathContext/PathContext.h"
#include "Core/Config/Resolver/Resolver.h"
#include "Core/Config/Resolver/Types/ResolverCandidate.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

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
