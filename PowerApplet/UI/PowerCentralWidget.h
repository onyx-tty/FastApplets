// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/CentralWidget.h"

#include <vector>
#include <QObject>
#include <Qt>

class PowerAppletConfig;
class PowerAppletKeys;
class PowerButton;
class QKeyEvent;
class QShowEvent;
class QWidget;

class PowerCentralWidget final : public CentralWidget {
        Q_OBJECT

private:
        std::vector<PowerButton*> buttons;

public:
        explicit PowerCentralWidget(const PowerAppletConfig& config, const PowerAppletKeys& keys,
                                    const PowerAppletKeys& default_keys, QWidget* parent);
        [[nodiscard]] const std::vector<PowerButton*>& getButtons() const;
};
