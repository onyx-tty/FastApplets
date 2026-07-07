// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AppletUIFactory.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/MainWindow.h"
#include "Core/UI/PrimaryButtonsFactory.h"
#include "Core/UI/Widgets/PrimaryButton.h"

#include <utility>

MainWindow AppletUIFactory::make(const Config& config, const Keys& keys, const Keys& default_keys) {
        auto primary_buttons =
                PrimaryButtonsFactory::create(config.getLayoutProperties().getPrimaryButtons(),
                                              config.getPrimaryButtonProperties(),
                                              keys.getPrimaryButtons(),
                                              default_keys.getPrimaryButtons(), nullptr);

        auto* central_widget =
                new CentralWidget(std::move(primary_buttons), keys.getQuit(),
                                  config.getPrimaryButtonProperties().getDoubleKeyPress(), nullptr);

        return MainWindow(config.getWindowProperties().getTitle(),
                          config.getWindowProperties().getSize(), central_widget);
}
