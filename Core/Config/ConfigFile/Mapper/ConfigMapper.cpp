// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigMapper.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Resolve/Types/ResolverCandidate.h"
#include "Core/UI/Types/LayoutProperties.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

using Candidates  = config::resolve::Candidates;
using PathContext = config::resolve::PathContext;

/* WindowParams */

WindowParams config::ConfigMapper::windowParams(const Candidates&   candidates,
                                                const WindowParams& defaults,
                                                const PathContext&  path_context) {
        return mapProperties(
                candidates, defaults, path_context,
                [&defaults, &candidates](WindowParams& window, const PathContext& path_context) {
                        window.size = resolve::from<QSize>(candidates.makeCopy().withExtension(
                                                                   "size"),
                                                           path_context.makeExtended("size"))
                                              .value_or(defaults.size);

                        window.title = resolve::from<QString>(candidates.makeCopy()
                                                                      .withExtension("title")
                                                                      .withQuiet(true, 1),
                                                              path_context.makeExtended("title"))
                                               .value_or(defaults.title);
                });
}

/* LayoutProperties */

PrimaryButtonStyle config::ConfigMapper::primaryButtonStyle(const Candidates&         candidates,
                                                            const PrimaryButtonStyle& defaults,
                                                            const PathContext& path_context) {
        return mapProperties(
                candidates, defaults, path_context,
                [&defaults, &candidates](PrimaryButtonStyle& button,
                                         const PathContext&  path_context) {
                        button.text_alignment =
                                resolve::from<Qt::Alignment>(candidates.makeCopy().withExtension(
                                                                     "text_alignment"),
                                                             path_context.makeExtended(
                                                                     "text_alignment"))
                                        .value_or(defaults.text_alignment);

                        button.icon_alignment =
                                resolve::from<Qt::Alignment>(candidates.makeCopy().withExtension(
                                                                     "icon_alignment"),
                                                             path_context.makeExtended(
                                                                     "icon_alignment"))
                                        .value_or(defaults.icon_alignment);

                        button.icon_size = resolve::from<QSize>(candidates.makeCopy().withExtension(
                                                                        "icon_size"),
                                                                path_context.makeExtended(
                                                                        "icon_size"))
                                                   .value_or(defaults.icon_size);

                        button.policy =
                                resolve::from<QSizePolicy>(candidates.makeCopy().withExtension(
                                                                   "policy"),
                                                           path_context.makeExtended("policy"))
                                        .value_or(defaults.policy);

                        button.double_key_press =
                                resolve::from<bool>(candidates.makeCopy().withExtension(
                                                            "double_key_press"),
                                                    path_context.makeExtended("double_key_press"))
                                        .value_or(defaults.double_key_press);
                });
}
