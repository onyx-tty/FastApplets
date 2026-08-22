// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Config.h"

#include "Core/Config/View/View.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

WindowParams config::map::helpers::windowParams(
        const ConfigView& node, const WindowParams& defaults) {
        return {.size  = node["size"].resolve<QSize>().value_or(defaults.size),
                .title = node["title"].resolve<QString>().value_or(defaults.title)};
}

PrimaryButtonStyle config::map::helpers::primaryButtonStyle(
        const ConfigView& node, const PrimaryButtonStyle& defaults) {
        return {.text_alignment = node["text_alignment"].resolve<Qt::Alignment>().value_or(
                        defaults.text_alignment),
                .icon_alignment = node["icon_alignment"].resolve<Qt::Alignment>().value_or(
                        defaults.icon_alignment),
                .icon_size = node["icon_size"].resolve<QSize>().value_or(defaults.icon_size),
                .policy    = node["policy"].resolve<QSizePolicy>().value_or(defaults.policy)};
}

PrimaryButtonBehavior config::map::helpers::primaryButtonBehavior(
        const ConfigView& node, const PrimaryButtonBehavior& defaults) {
        return {.double_key_press = node["double_key_press"].resolve<bool>().value_or(
                        defaults.double_key_press)};
}
