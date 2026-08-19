// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"

#include "Core/Config/Map/Helpers/Helpers.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

WindowParams config::map::helpers::windowParams(
        const Candidates& candidates, const WindowParams& defaults) {
        return table<WindowParams>(candidates, [&defaults, &candidates](WindowParams& window) {
                field(window, &WindowParams::size, candidates, defaults, u"size");
                field(window, &WindowParams::title, candidates, defaults, u"title");
        }).value_or(defaults);
}

PrimaryButtonStyle config::map::helpers::primaryButtonStyle(
        const Candidates& candidates, const PrimaryButtonStyle& defaults) {
        return table<PrimaryButtonStyle>(candidates, [&defaults, &candidates](
                                                             PrimaryButtonStyle& button) {
                field(button, &PrimaryButtonStyle::text_alignment, candidates, defaults,
                        u"text_alignment");
                field(button, &PrimaryButtonStyle::icon_alignment, candidates, defaults,
                        u"icon_alignment");
                field(button, &PrimaryButtonStyle::icon_size, candidates, defaults, u"icon_size");
                field(button, &PrimaryButtonStyle::policy, candidates, defaults, u"policy");
        }).value_or(defaults);
}

PrimaryButtonBehavior config::map::helpers::primaryButtonBehavior(
        const Candidates& candidates, const PrimaryButtonBehavior& defaults) {
        return table<PrimaryButtonBehavior>(candidates, [&defaults, &candidates](
                                                                PrimaryButtonBehavior& button) {
                field(button, &PrimaryButtonBehavior::double_key_press, candidates, defaults,
                        u"double_key_press");
        }).value_or(defaults);
}
