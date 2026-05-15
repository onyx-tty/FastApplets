// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UI/MainWindow.h"

#include <QObject>

class PowerCentralWidget;

class PowerMainWindow final : public MainWindow {
        Q_OBJECT

private:
        PowerCentralWidget* central_widget;

public:
        explicit PowerMainWindow();
        const PowerCentralWidget* getCentralWidget() const;
};
