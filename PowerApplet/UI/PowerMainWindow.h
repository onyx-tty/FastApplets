// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/MainWindow.h"

#include <QObject>
#include <QWidget>

class PowerCentralWidget;

class PowerMainWindow final : public MainWindow {
        Q_OBJECT

public:
        explicit PowerMainWindow(QWidget* parent = nullptr);
};
