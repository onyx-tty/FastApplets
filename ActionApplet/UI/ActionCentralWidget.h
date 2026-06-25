// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/CentralWidget.h"

#include <vector>
#include <QObject>
#include <Qt>

class ActionButton;
class QWidget;

class ActionCentralWidget final : public CentralWidget {
        Q_OBJECT

private:
        std::vector<ActionButton*> buttons;

public:
        explicit ActionCentralWidget(std::vector<ActionButton*> buttons,
                                     const keybindings& quit_keys, bool double_key_press,
                                     QWidget* parent);
        [[nodiscard]] const std::vector<ActionButton*>& getButtons() const;
};
