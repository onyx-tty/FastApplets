// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerCentralWidget.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Core/Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/PrimaryButtonsFactory.h"
#include "PowerApplet/Types/PowerAppletTraits.h"
#include "Widgets/PowerButton.h"

#include <vector>
#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>
#include <Qt>
#include <QtGlobal>

PowerCentralWidget::PowerCentralWidget(const PowerAppletConfig& config, const PowerAppletKeys& keys,
                                       const PowerAppletKeys& default_keys, QWidget* parent) :
        CentralWidget({}, keys.getQuit(), config.getPrimaryButtonProperties().getDoubleKeyPress(),
                      parent) {
        CentralWidget::buttons =
                PrimaryButtonsFactory<applet::type::power_applet>::create(config, keys,
                                                                          default_keys, this);

        // Caches pointers to PowerButtons stored in CentralWidget.
        buttons.reserve(CentralWidget::buttons.size());
        for (auto* button : CentralWidget::buttons) {
                layout()->addWidget(button);

                if (auto* power = dynamic_cast<PowerButton*>(button)) { buttons.push_back(power); }
        }
}

const std::vector<PowerButton*>& PowerCentralWidget::getButtons() const {
        return buttons;
}
