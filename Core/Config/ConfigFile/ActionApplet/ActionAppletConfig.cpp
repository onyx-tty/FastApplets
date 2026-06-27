// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionAppletConfig.h"
#include "Core/Config/ConfigFile/Global/GlobalConfig.h"
#include "Core/Config/ConfigFile/Mapper/ConfigMapper.h"
#include "Core/Config/ConfigFile/Properties/LayoutProperties.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"

#include <utility>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>

ActionAppletConfig::ActionAppletConfig(WindowProperties        window,
                                       PrimaryButtonProperties primary_button,
                                       LayoutProperties        layout) :
        GlobalConfig(std::move(window), std::move(primary_button)),
        layout_properties(std::move(layout)) {}

const LayoutProperties& ActionAppletConfig::getLayoutProperties() const {
        return layout_properties;
}
