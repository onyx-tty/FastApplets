// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/CentralWidget.h"

#include <vector>
#include <QObject>
#include <Qt>

class ActionAppletConfig;
class ActionAppletKeys;
class ActionButton;
class QKeyEvent;
class QShowEvent;
class QWidget;

class ActionCentralWidget final : public CentralWidget {
        Q_OBJECT

private:
        std::vector<ActionButton*> buttons;

public:
        explicit ActionCentralWidget(const ActionAppletConfig& config, const ActionAppletKeys& keys,
                                     const ActionAppletKeys& default_keys, QWidget* parent);
        [[nodiscard]] const std::vector<ActionButton*>& getButtons() const;
};
