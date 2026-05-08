// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Config/ConfigFile/Properties/WindowProperties.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>

// Base config holding properties common to all applets.
//
// Derive from this and extend with applet-specific properties.
class GlobalConfig {
protected:
        explicit GlobalConfig(WindowProperties        window_properties,
                              PrimaryButtonProperties primary_button_properties);

        WindowProperties        window_properties;
        PrimaryButtonProperties primary_button_properties;

public:
        const WindowProperties&        getWindowProperties() const;
        const PrimaryButtonProperties& getPrimaryButtonProperties() const;
};
