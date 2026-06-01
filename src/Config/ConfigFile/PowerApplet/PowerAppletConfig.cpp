// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerAppletConfig.h"
#include "Config/ConfigFile/Global/GlobalConfig.h"
#include "Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Config/ConfigFile/Properties/LayoutProperties.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

PowerAppletConfig::PowerAppletConfig(WindowProperties        window,
                                     PrimaryButtonProperties primary_button,
                                     LayoutProperties        layout) :
        GlobalConfig(std::move(window), std::move(primary_button)),
        layout_properties(std::move(layout)) {}

const LayoutProperties& PowerAppletConfig::getLayoutProperties() const {
        return layout_properties;
}
