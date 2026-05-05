// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

class GlobalConfig {
protected:
        friend class ConfigMapper;

        explicit GlobalConfig(
                WindowProperties        window_properties         = WindowProperties{},
                PrimaryButtonProperties primary_button_properties = PrimaryButtonProperties{});

        WindowProperties        window_properties;
        PrimaryButtonProperties primary_button_properties;

public:
        const WindowProperties&        getWindowProperties() const;
        const PrimaryButtonProperties& getPrimaryButtonProperties() const;
};
