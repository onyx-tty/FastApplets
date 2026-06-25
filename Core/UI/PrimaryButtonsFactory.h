// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"

#include <vector>

class PrimaryButton;
class PrimaryButtonProperties;
class QWidget;

// TODO: Document
template<applet::type TApplet>
class PrimaryButtonsFactory final {
private:
        using TPrimaryButton       = AppletTraits<TApplet>::TPrimaryButton;
        using TPrimaryButtonParams = AppletTraits<TApplet>::TPrimaryButtonParams;

public:
        PrimaryButtonsFactory() = delete;

        // Creates and registers all PrimaryButtons from Config and Keys.
        //
        // Reads buttons and definitions from config and keybindings from
        // Keys, constructs a PrimaryButton widget for each entry,
        // adds it to the layout, and returns a vector with every button found.
        //
        // WARNING: If a resolved key is already bound elsewhere, both buttons
        // will share it silently. A global keybinding validation pass is not
        // implemented yet.
        //
        // Keybinding resolution order:
        // 1. User-configured keys (keys).
        // 2. Default keys (default_keys).
        // 3. Qt::Key_unknown if the defaults are exhausted.
        //
        // Returns a vector containing every created button.
        // Calls qFatal if no buttons are found in config.
        static std::vector<TPrimaryButton*> create(const std::vector<TPrimaryButtonParams>& params,
                                                   const PrimaryButtonProperties&  properties,
                                                   const std::vector<keybindings>& keys,
                                                   const std::vector<keybindings>& default_keys,
                                                   QWidget*                        parent);
};

#include "PrimaryButtonsFactory.tpp"
