// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/CentralWidget.h"

#include <vector>
#include <QObject>
#include <Qt>

class PowerButton;
class QWidget;

class PowerCentralWidget final : public CentralWidget {
        Q_OBJECT

private:
        std::vector<PowerButton*> buttons;

public:
        explicit PowerCentralWidget(std::vector<PowerButton*> buttons, const keybindings& quit_keys,
                                    bool double_key_press, QWidget* parent);
        [[nodiscard]] const std::vector<PowerButton*>& getButtons() const;
};
