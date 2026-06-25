// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/MainWindow.h"

#include <vector>
#include <QObject>
#include <QWidget>

class ActionCentralWidget;
class ActionButton;
class ActionAppletConfig;
class ActionAppletKeys;
class QString;
class QSize;

// Unpacks WindowProperties to set window size and title.
class ActionMainWindow final : public MainWindow {
        Q_OBJECT

public:
        explicit ActionMainWindow(const QString& title, QSize size,
                                  std::vector<ActionButton*> buttons, const keybindings& quit_keys,
                                  bool double_key_press, QWidget* parent = nullptr);
};
