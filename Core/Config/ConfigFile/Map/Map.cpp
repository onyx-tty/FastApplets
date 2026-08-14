// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Map.h"
#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <toml++/toml.hpp>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

/* WindowParams */

WindowParams config::map::windowParams(const Candidates& candidates, const WindowParams& defaults) {
        return table<WindowParams>(candidates,
                                   [&defaults, &candidates](WindowParams& window) {
                                           window.size = resolve::from<QSize>(
                                                                 candidates.makeCopy().withExtension(
                                                                         u"size"))
                                                                 .value_or(defaults.size);

                                           window.title = resolve::from<QString>(
                                                                  candidates.makeCopy()
                                                                          .withExtension(u"title"))
                                                                  .value_or(defaults.title);
                                   })
                .value_or(defaults);
}

/* PrimaryButtonParams */

PrimaryButtonStyle config::map::primaryButtonStyle(const Candidates&         candidates,
                                                   const PrimaryButtonStyle& defaults) {
        return table<PrimaryButtonStyle>(
                       candidates,
                       [&defaults, &candidates](PrimaryButtonStyle& button) {
                               button.text_alignment = resolve::from<Qt::Alignment>(
                                                               candidates.makeCopy().withExtension(
                                                                       u"text_alignment"))
                                                               .value_or(defaults.text_alignment);

                               button.icon_alignment = resolve::from<Qt::Alignment>(
                                                               candidates.makeCopy().withExtension(
                                                                       u"icon_alignment"))
                                                               .value_or(defaults.icon_alignment);

                               button.icon_size = resolve::from<QSize>(
                                                          candidates.makeCopy().withExtension(
                                                                  u"icon_size"))
                                                          .value_or(defaults.icon_size);

                               button.policy = resolve::from<QSizePolicy>(
                                                       candidates.makeCopy().withExtension(
                                                               u"policy"))
                                                       .value_or(defaults.policy);
                       })
                .value_or(defaults);
}

PrimaryButtonBehavior config::map::primaryButtonBehavior(const Candidates&            candidates,
                                                         const PrimaryButtonBehavior& defaults) {
        return table<PrimaryButtonBehavior>(
                       candidates,
                       [&defaults, &candidates](PrimaryButtonBehavior& button) {
                               button.double_key_press =
                                       resolve::from<bool>(candidates.makeCopy().withExtension(
                                                                   u"double_key_press"))
                                               .value_or(defaults.double_key_press);
                       })
                .value_or(defaults);
}
