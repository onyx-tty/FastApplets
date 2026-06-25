// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerCentralWidget.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/Widgets/PrimaryButton.h"
#include "Widgets/PowerButton.h"

#include <utility>
#include <vector>
#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>
#include <Qt>
#include <QtGlobal>

PowerCentralWidget::PowerCentralWidget(std::vector<PowerButton*> buttons,
                                       const keybindings& quit_keys, bool double_key_press,
                                       QWidget* parent) :
        CentralWidget({}, quit_keys, double_key_press, parent), buttons(std::move(buttons)) {
        // Fills PrimaryButton pointers in CentralWidget for polymorphism.
        // It's a temporary solution to code repetition between central widgets.
        CentralWidget::buttons.reserve(buttons.size());
        for (auto* button : this->buttons) {
                // Adds each button to the layout.
                layout()->addWidget(button);

                if (auto* power = dynamic_cast<PrimaryButton*>(button)) {
                        CentralWidget::buttons.push_back(power);
                }
        }
}

const std::vector<PowerButton*>& PowerCentralWidget::getButtons() const {
        return buttons;
}
