// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/MainWindow.h"

#include <QObject>
#include <QWidget>

class PowerCentralWidget;
class PowerAppletConfig;
class PowerAppletKeys;

class PowerMainWindow final : public MainWindow {
        Q_OBJECT

public:
        explicit PowerMainWindow(const PowerAppletConfig& config, const PowerAppletKeys& keys,
                                 const PowerAppletKeys& default_keys, QWidget* parent = nullptr);
};
