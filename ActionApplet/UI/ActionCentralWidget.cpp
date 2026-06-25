// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionCentralWidget.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/Widgets/PrimaryButton.h"
#include "Widgets/ActionButton.h"

#include <utility>
#include <vector>
#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>
#include <Qt>
#include <QtGlobal>

ActionCentralWidget::ActionCentralWidget(std::vector<ActionButton*> buttons,
                                         const keybindings& quit_keys, bool double_key_press,
                                         QWidget* parent) :
        CentralWidget({}, quit_keys, double_key_press, parent), buttons(std::move(buttons)) {
        // Fills PrimaryButton pointers in CentralWidget for polymorphism.
        // It's a temporary solution to code repetition between central widgets.
        CentralWidget::buttons.reserve(buttons.size());
        for (auto* button : this->buttons) {
                // Adds each button to the layout.
                layout()->addWidget(button);

                if (auto* action = dynamic_cast<PrimaryButton*>(button)) {
                        CentralWidget::buttons.push_back(action);
                }
        }
}

const std::vector<ActionButton*>& ActionCentralWidget::getButtons() const {
        return buttons;
}
