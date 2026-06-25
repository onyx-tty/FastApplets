// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/MainWindow.h"

#include <vector>
#include <QObject>
#include <QWidget>

class PowerCentralWidget;
class PowerButton;
class PowerAppletConfig;
class PowerAppletKeys;
class QString;
class QSize;

// Unpacks WindowProperties to set window size and title.
class PowerMainWindow final : public MainWindow {
        Q_OBJECT

public:
        explicit PowerMainWindow(const QString& title, QSize size,
                                 std::vector<PowerButton*> buttons, const keybindings& quit_keys,
                                 bool double_key_press, QWidget* parent = nullptr);
};
