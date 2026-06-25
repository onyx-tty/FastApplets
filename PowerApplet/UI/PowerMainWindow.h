// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/MainWindow.h"

#include <vector>
#include <QObject>
#include <QWidget>

class PowerCentralWidget;
class PowerButton;
class PowerAppletConfig;
class PowerAppletKeys;

// Unpacks WindowProperties to set window size and title.
class PowerMainWindow final : public MainWindow {
        Q_OBJECT

public:
        explicit PowerMainWindow(std::vector<PowerButton*> buttons, const PowerAppletConfig& config,
                                 const PowerAppletKeys& keys, const PowerAppletKeys& default_keys,
                                 QWidget* parent = nullptr);
};
