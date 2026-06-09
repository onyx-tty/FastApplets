// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/UI/MainWindow.h"

#include <QObject>
#include <QWidget>

class ActionCentralWidget;
class ActionAppletConfig;
class ActionAppletKeys;

// Unpacks WindowProperties to set window size and title.
class ActionMainWindow final : public MainWindow {
        Q_OBJECT

public:
        explicit ActionMainWindow(const ActionAppletConfig& config, const ActionAppletKeys& keys,
                                  const ActionAppletKeys& default_keys, QWidget* parent = nullptr);
};
