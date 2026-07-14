// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConfigMapper.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/Config/Resolve/PathContext/PathContext.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Resolve/Types/ResolverCandidate.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

using Window        = config::schema::properties::Window;
using PrimaryButton = config::schema::properties::PrimaryButton;
using Layout        = config::schema::properties::Layout;
using Candidates    = config::resolve::Candidates;
using PathContext   = config::resolve::PathContext;

/* Window Properties */

Window config::ConfigMapper::window(const Candidates& candidates, const Window& defaults,
                                    const PathContext& path_context) {
        return mapProperties(
                candidates, defaults, path_context,
                [&defaults, &candidates](Window& window, const PathContext& path_context) {
                        window.size = resolve::from<QSize>(candidates.makeCopy().withExtension(
                                                                   "size"),
                                                           path_context.makeExtended("size"))
                                              .value_or(defaults.getSize());

                        window.title = resolve::from<QString>(candidates.makeCopy()
                                                                      .withExtension("title")
                                                                      .withQuiet(true, 1),
                                                              path_context.makeExtended("title"))
                                               .value_or(defaults.getTitle());
                });
}

/* Primary Button Properties*/

PrimaryButton config::ConfigMapper::primaryButton(const Candidates&    candidates,
                                                  const PrimaryButton& defaults,
                                                  const PathContext&   path_context) {
        return mapProperties(
                candidates, defaults, path_context,
                [&defaults, &candidates](PrimaryButton& button, const PathContext& path_context) {
                        button.double_key_press =
                                resolve::from<bool>(candidates.makeCopy().withExtension(
                                                            "double_key_press"),
                                                    path_context.makeExtended("double_key_press"))
                                        .value_or(defaults.getDoubleKeyPress());

                        button.text_alignment =
                                resolve::from<Qt::Alignment>(candidates.makeCopy().withExtension(
                                                                     "text_alignment"),
                                                             path_context.makeExtended(
                                                                     "text_alignment"))
                                        .value_or(defaults.getTextAlignment());

                        button.icon_alignment =
                                resolve::from<Qt::Alignment>(candidates.makeCopy().withExtension(
                                                                     "icon_alignment"),
                                                             path_context.makeExtended(
                                                                     "icon_alignment"))
                                        .value_or(defaults.getIconAlignment());

                        button.icon_size = resolve::from<QSize>(candidates.makeCopy().withExtension(
                                                                        "icon_size"),
                                                                path_context.makeExtended(
                                                                        "icon_size"))
                                                   .value_or(defaults.getIconSize());

                        button.policy =
                                resolve::from<QSizePolicy>(candidates.makeCopy().withExtension(
                                                                   "policy"),
                                                           path_context.makeExtended("policy"))
                                        .value_or(defaults.getPolicy());
                });
}
