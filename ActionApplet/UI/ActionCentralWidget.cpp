// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionCentralWidget.h"
#include "ActionApplet/Types/ActionAppletTraits.h"
#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/ConfigFile/ActionApplet/ActionAppletConfig.h"
#include "Core/Config/KeysFile/ActionApplet/ActionAppletKeys.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/PrimaryButtonsFactory.h"
#include "Widgets/ActionButton.h"

#include <vector>
#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>
#include <Qt>
#include <QtGlobal>

// TODO: Accept button as param for dependency injection

ActionCentralWidget::ActionCentralWidget(const ActionAppletConfig& config,
                                         const ActionAppletKeys&   keys,
                                         const ActionAppletKeys& default_keys, QWidget* parent) :
        CentralWidget({}, keys.getQuit(), config.getPrimaryButtonProperties().getDoubleKeyPress(),
                      parent) {
        CentralWidget::buttons =
                PrimaryButtonsFactory<applet::type::action_applet>::create(config, keys,
                                                                           default_keys, this);

        // Caches pointers to ActionButtons stored in CentralWidget.
        buttons.reserve(CentralWidget::buttons.size());
        for (auto* button : CentralWidget::buttons) {
                layout()->addWidget(button);

                if (auto* action = dynamic_cast<ActionButton*>(button)) {
                        buttons.push_back(action);
                }
        }
}

const std::vector<ActionButton*>& ActionCentralWidget::getButtons() const {
        return buttons;
}
